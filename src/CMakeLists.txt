file(GLOB SOURCES "*.cpp" "*.c")
add_library(ml_uuid SHARED ${SOURCES})

if(WIN32)
    # 1. Wska¿ folder z bibliotekami (Additional Library Directories)
    target_link_directories(ml_uuid PRIVATE "${PROJECT_SOURCE_DIR}/lib")
    
    # 2. Linkuj sam¹ nazwê (Additional Dependencies)
    # Upewnij siê, ¿e nazwa pasuje do pliku w folderze /lib/ (bez lub z .lib)
    target_link_libraries(ml_uuid PRIVATE lua5.1) 
    
    # 3. Definicje dla MTA i Lua
    target_compile_definitions(ml_uuid PRIVATE LUA_BUILD_AS_DLL)
endif()

target_include_directories(ml_uuid PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(ml_uuid PRIVATE 
    luaimports
    Boost::uuid
)