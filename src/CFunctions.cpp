#include "CFunctions.h"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/time_generator_v7.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <cstring>
#include <mutex>

namespace
{
template <typename Generator>
int pushUuid(lua_State* luaVM)
{
    if (!luaVM) return 0;

    char text[36];
    bool failed = false;
    try
    {
        // Preserve generator state across calls and resource Lua VMs.
        // Lock only generation; never enter Lua with a C++ lock/string alive.
        static std::mutex mutex;
        static Generator generator;
        const std::lock_guard<std::mutex> lock(mutex);
        const auto value = boost::uuids::to_string(generator());
        std::memcpy(text, value.data(), sizeof(text));
    }
    catch (...)
    {
        // Keep C++ exceptions inside the module boundary.
        failed = true;
    }

    // Lua 5.1 can longjmp on allocation failure. All non-trivial automatic
    // objects above have already been destroyed before these Lua API calls.
    if (failed)
    {
        lua_pushnil(luaVM);
        lua_pushliteral(luaVM, "UUID generation failed");
        return 2;
    }

    lua_pushlstring(luaVM, text, sizeof(text));
    return 1;
}
}

int CFunctions::uuid4(lua_State* luaVM)
{
    return pushUuid<boost::uuids::random_generator>(luaVM);
}

int CFunctions::uuid7(lua_State* luaVM)
{
    return pushUuid<boost::uuids::time_generator_v7>(luaVM);
}
