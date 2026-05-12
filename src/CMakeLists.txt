file(GLOB SOURCES "*.cpp" "*.c")

add_library(ml_uuid SHARED ${SOURCES})

# ... (reszta Twojego CMakeLists)

if(WIN32)
    target_link_directories(ml_uuid PRIVATE "${PROJECT_SOURCE_DIR}/lib")
    target_link_libraries(ml_uuid PRIVATE lua5.1)
    target_compile_definitions(ml_uuid PRIVATE LUA_BUILD_AS_DLL)
else()
    # LINUX - Kluczowe flagi
    target_compile_options(ml_uuid PRIVATE -fPIC)
    
    # Ta flaga mówi linkerowi: "Nie martw siê o brakuj¹ce funkcje lua_..., 
    # zostan¹ znalezione w mta-server podczas ³adowania"
    set_target_properties(ml_uuid PROPERTIES 
        PREFIX ""
        LINK_FLAGS "-Wl,--unresolved-symbols=ignore-all"
    )

    # Dodaj biblioteki systemowe wymagane przez kod
    target_link_libraries(ml_uuid PRIVATE dl atomic)
endif()

target_include_directories(ml_uuid PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(ml_uuid PRIVATE 
    luaimports
    Boost::uuid
)