cmake_minimum_required(VERSION 3.16)
project(ml_uuid VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Integracja z vcpkg (dla Boost::uuid)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Znajdü Boost
find_package(Boost REQUIRED COMPONENTS uuid)

# Dodaj tylko folder src. 
# Folder luaimports zostanie obs≥uøony warunkowo wewnπtrz src/CMakeLists.txt
add_subdirectory(src)