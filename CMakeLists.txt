cmake_minimum_required(VERSION 3.14)
project(ml_uuid VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Additional packages
find_package(Boost REQUIRED COMPONENTS uuid)

add_subdirectory(luaimports)
add_subdirectory(src)

set_target_properties(ml_uuid PROPERTIES PREFIX "")
set_target_properties(ml_uuid PROPERTIES OUTPUT_NAME "ml_uuid")