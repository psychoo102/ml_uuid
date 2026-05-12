file(GLOB SOURCES "*.cpp" "*.c")
add_library(ml_uuid SHARED ${SOURCES})

if(WIN32)
    # Na Windows MUSIMY linkowaæ do .lib
    target_link_directories(ml_uuid PRIVATE "${PROJECT_SOURCE_DIR}/lib")
    target_link_libraries(ml_uuid PRIVATE lua5.1)
    target_compile_definitions(ml_uuid PRIVATE LUA_BUILD_AS_DLL)
else()
    # Na Linuxie TYLKO nag³ówki. Symbole zostan¹ rozwi¹zane przez mta-server
    find_package(Lua 5.1 REQUIRED)
    target_include_directories(ml_uuid PRIVATE ${LUA_INCLUDE_DIR})
    
    # NIE dodawaj ${LUA_LIBRARIES} do target_link_libraries!
    # Zamiast tego dodaj te flagi, aby linker nie krzycza³ o brakuj¹cych symbolach
    set_target_properties(ml_uuid PROPERTIES LINK_FLAGS "-Wl,--unresolved-symbols=ignore-in-shared-libs")
    
    # Usuñ prefiks lib
    set_target_properties(ml_uuid PROPERTIES PREFIX "")
endif()

target_include_directories(ml_uuid PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(ml_uuid PRIVATE 
    luaimports
    Boost::uuid
)