// Tests the *actual production callback implementation* without linking MTA.
// Including the .cpp in this dedicated test translation unit also exposes the
// private pushUuid<Generator>() template for deterministic fault injection.
#include "../../src/CFunctions.cpp"

#include <chrono>
#include <cstdint>
#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

struct TestLuaStack
{
    std::vector<std::optional<std::string>> pushed;
};

extern "C" void lua_pushnil(lua_State* state)
{
    reinterpret_cast<TestLuaStack*>(state)->pushed.emplace_back(std::nullopt);
}

extern "C" void lua_pushlstring(lua_State* state, const char* text, size_t length)
{
    reinterpret_cast<TestLuaStack*>(state)->pushed.emplace_back(std::string(text, length));
}

namespace
{
void require(bool ok, const char* message)
{
    if (!ok) throw std::runtime_error(message);
}

lua_State* vm(TestLuaStack& stack)
{
    return reinterpret_cast<lua_State*>(&stack);
}

std::string call(int (*callback)(lua_State*), TestLuaStack& stack)
{
    stack.pushed.clear();
    require(callback(vm(stack)) == 1, "UUID callback must return one Lua result");
    require(stack.pushed.size() == 1 && stack.pushed[0].has_value(),
            "UUID callback did not push a string");
    return *stack.pushed[0];
}

void validate(const std::string& id, char version)
{
    require(id.size() == 36, "UUID must have 36 characters");
    for (size_t i = 0; i < id.size(); ++i)
    {
        if (i == 8 || i == 13 || i == 18 || i == 23)
        {
            require(id[i] == '-', "UUID separator is missing");
        }
        else
        {
            require((id[i] >= '0' && id[i] <= '9') ||
                    (id[i] >= 'a' && id[i] <= 'f'), "UUID is not lowercase hex");
        }
    }
    require(id[14] == version, "UUID has an incorrect version");
    require(id[19] == '8' || id[19] == '9' || id[19] == 'a' || id[19] == 'b',
            "UUID does not have the RFC variant");
}

std::uint64_t timestampMs(const std::string& id)
{
    return std::stoull(id.substr(0, 8) + id.substr(9, 4), nullptr, 16);
}

std::int64_t nowMs()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void testSingleThread()
{
    TestLuaStack firstVm;
    TestLuaStack secondVm;
    std::unordered_set<std::string> seenV4;
    std::unordered_set<std::string> seenV7;
    const auto before = nowMs();
    for (int i = 0; i < 5000; ++i)
    {
        // Alternating Lua states must not reset the module's generator state.
        auto& stack = (i % 2 == 0) ? firstVm : secondVm;
        const auto v4 = call(CFunctions::uuid4, stack);
        const auto v7 = call(CFunctions::uuid7, stack);
        validate(v4, '4');
        validate(v7, '7');
        require(seenV4.insert(v4).second, "Duplicate UUIDv4 in test sample");
        require(seenV7.insert(v7).second, "Duplicate UUIDv7 in test sample");
        const auto stamp = static_cast<std::int64_t>(timestampMs(v7));
        require(stamp >= before - 5000 && stamp <= nowMs() + 5000,
                "UUIDv7 timestamp does not match wall-clock time");
    }
}

void testConcurrentCalls()
{
    constexpr int threads = 8;
    constexpr int perThread = 1000;
    std::vector<std::thread> workers;
    std::vector<std::exception_ptr> failures(threads);
    std::vector<std::vector<std::string>> values(threads);

    for (int t = 0; t < threads; ++t)
    {
        workers.emplace_back([&, t]()
        {
            try
            {
                TestLuaStack stack;
                auto& collected = values[t];
                collected.reserve(perThread * 2);
                for (int i = 0; i < perThread; ++i)
                {
                    const auto v4 = call(CFunctions::uuid4, stack);
                    const auto v7 = call(CFunctions::uuid7, stack);
                    validate(v4, '4');
                    validate(v7, '7');
                    collected.push_back(v4);
                    collected.push_back(v7);
                }
            }
            catch (...)
            {
                failures[t] = std::current_exception();
            }
        });
    }
    for (auto& worker : workers) worker.join();
    for (const auto& failure : failures)
        if (failure) std::rethrow_exception(failure);

    std::unordered_set<std::string> all;
    for (const auto& batch : values)
        for (const auto& id : batch)
            require(all.insert(id).second, "Duplicate UUID across concurrent callbacks");
}

struct StatefulGenerator
{
    unsigned char counter = 0;

    boost::uuids::uuid operator()()
    {
        boost::uuids::uuid value{};
        value.data[15] = ++counter;
        return value;
    }
};

struct ThrowingGenerator
{
    boost::uuids::uuid operator()() const
    {
        throw std::runtime_error("injected generator failure");
    }
};

void testGeneratorStateAndErrors()
{
    TestLuaStack firstVm;
    TestLuaStack secondVm;
    // The production template has a static generator per Generator type.
    const auto first = call(pushUuid<StatefulGenerator>, firstVm);
    const auto second = call(pushUuid<StatefulGenerator>, secondVm);
    require(first != second, "Generator was re-created between Lua VMs");

    firstVm.pushed.clear();
    require(pushUuid<ThrowingGenerator>(vm(firstVm)) == 2,
            "Failure must return two Lua values");
    require(firstVm.pushed.size() == 2 && !firstVm.pushed[0].has_value() &&
            firstVm.pushed[1] == std::optional<std::string>("UUID generation failed"),
            "Failure must return nil plus a stable error string");
    require(CFunctions::uuid4(nullptr) == 0 && CFunctions::uuid7(nullptr) == 0,
            "Null Lua VM should return without touching Lua");
}
}

int main()
{
    try
    {
        testSingleThread();
        testConcurrentCalls();
        testGeneratorStateAndErrors();
        std::cout << "PASS: UUID callbacks, versions, timestamps, uniqueness, "
                     "concurrency, persistent state and error handling\n";
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
