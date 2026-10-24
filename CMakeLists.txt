cmake_minimum_required(VERSION 3.21)

# The toolchain must be selected before project() initializes compilers.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "")
endif()

project(ml_uuid VERSION 1.1.3 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.86 REQUIRED COMPONENTS uuid)
find_package(Threads REQUIRED)
include(CTest)
add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(tests/native)
endif()
