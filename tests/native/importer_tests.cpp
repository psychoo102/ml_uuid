// Exercise the production Linux importer without loading a real MTA core.
// GNU ld wraps only the dynamic-loader calls made by this test executable.
#include "ILuaModuleManager.h"
#include "Common.h"
#include <dlfcn.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

extern "C" bool ImportLua();

namespace
{
int coreToken;
int symbolToken;
int opens = 0;
int closes = 0;
int lookups = 0;
int diagnostics = 0;
bool failOpen = false;
const char* missingSymbol = nullptr;

void require(bool ok, const char* message)
{
    if (!ok) throw std::runtime_error(message);
}

class TestManager final : public ILuaModuleManager
{
public:
    void ErrorPrintf(const char*, ...) override { ++diagnostics; }
    void DebugPrintf(lua_State*, const char*, ...) override {}
    void Printf(const char*, ...) override { ++diagnostics; }
    bool RegisterFunction(lua_State*, const char*, lua_CFunction) override { return true; }
    bool GetResourceName(lua_State*, std::string&) override { return false; }
    CChecksum GetResourceMetaChecksum(lua_State*) override { return {}; }
    CChecksum GetResourceFileChecksum(lua_State*, const char*) override { return {}; }
};

TestManager manager;

void reset(bool cannotOpen = false, const char* missing = nullptr)
{
    opens = closes = lookups = diagnostics = 0;
    failOpen = cannotOpen;
    missingSymbol = missing;
}
}

ILuaModuleManager* pModuleManager = &manager;

extern "C" void* __wrap_dlopen(const char* path, int flags)
{
#ifdef ANY_x64
    require(std::strcmp(path, "x64/deathmatch.so") == 0, "Incorrect x64 core path");
#else
    require(std::strcmp(path, "mods/deathmatch/deathmatch.so") == 0, "Incorrect x86 core path");
#endif
    require(flags == (RTLD_NOW | RTLD_NOLOAD), "Importer must only acquire the loaded core");
    ++opens;
    return failOpen ? nullptr : &coreToken;
}

extern "C" void* __wrap_dlsym(void* handle, const char* symbol)
{
    require(handle == &coreToken, "Symbol lookup used the wrong core handle");
    ++lookups;
    return missingSymbol && std::strcmp(symbol, missingSymbol) == 0 ? nullptr : &symbolToken;
}

extern "C" int __wrap_dlclose(void* handle)
{
    require(handle == &coreToken, "Closing the wrong core handle");
    require(closes < opens, "Core handle closed more than once");
    ++closes;
    return 0;
}

extern "C" char* __wrap_dlerror()
{
    static char message[] = "injected loader failure";
    return message;
}

int main()
{
    try
    {
        reset();
        require(ImportLua(), "Complete core imports must succeed");
        require(opens == 1 && closes == 1 && lookups > 0 && diagnostics == 0,
                "Successful import must release exactly one reference");

        reset(true);
        require(!ImportLua(), "Missing core must fail");
        require(opens == 1 && closes == 0 && lookups == 0 && diagnostics == 1,
                "Failed acquisition must not close or inspect a null handle");

        for (const char* symbol : {"lua_newstate", "lua_pushlstring", "luaL_pushresult"})
        {
            reset(false, symbol);
            require(!ImportLua(), "Missing required symbol must fail");
            require(opens == 1 && closes == 1 && lookups > 0 && diagnostics == 1,
                    "Missing-symbol return must release its reference");
        }

        reset();
        for (int i = 0; i < 100; ++i)
        {
            missingSymbol = i % 2 ? "lua_pushlstring" : nullptr;
            require(ImportLua() == (missingSymbol == nullptr), "Unexpected repeated import result");
            require(opens == i + 1 && closes == opens,
                    "Repeated import attempts must not accumulate core references");
        }
        std::cout << "PASS: Linux importer reference lifetime and failure paths\n";
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
