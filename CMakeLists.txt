cmake_minimum_required(VERSION 3.14)

project(ml_uuid VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" 
        CACHE STRING "Vcpkg toolchain")
endif()

set(LUA_LIB_DIR "${CMAKE_SOURCE_DIR}/lib")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(LUA_LIBRARY "${LUA_LIB_DIR}/x64/lua5.1.lib")
    message(STATUS "Lua: Using x64/lua5.1.lib")
else()
    set(LUA_LIBRARY "${LUA_LIB_DIR}/lua5.1.lib")
    set(VCPKG_TARGET_TRIPLET "x86-windows" CACHE STRING "")
endif()

add_library(lua51 INTERFACE)

target_include_directories(lua51 INTERFACE 
    "${CMAKE_SOURCE_DIR}/luaimports"
)

target_link_libraries(lua51 INTERFACE "${LUA_LIBRARY}")

add_subdirectory(luaimports)
add_subdirectory(src)