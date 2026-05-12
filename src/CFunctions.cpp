#include "CFunctions.h"
#include <random>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

// ====================== UUID v4 ======================
int CFunctions::uuid4(lua_State* luaVM)
{
    if (!luaVM) return 0;
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	std::string uuid_str = boost::uuids::to_string(uuid);
	lua_pushstring(luaVM, uuid_str.c_str());
    return 1;
}

// ====================== UUID v7 ======================
int CFunctions::uuid7(lua_State* luaVM)
{
    if (!luaVM) return 0;
    //uuid const id = uuids::uuid_system_generator{}();
    boost::uuids::uuid uuid = boost::uuids::time_generator_v7()();
	std::string uuid_str = boost::uuids::to_string(uuid);
	lua_pushstring(luaVM, uuid_str.c_str());
    return 1;
}