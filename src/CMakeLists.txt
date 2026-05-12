file(GLOB CORE_SOURCES "*.cpp" "*.c")

if(WIN32)
    set(FINAL_SOURCES ${CORE_SOURCES})
else()
    file(GLOB LUA_IMPORTS_SOURCES "${CMAKE_SOURCE_DIR}/luaimports/*.cpp")
    set(FINAL_SOURCES ${CORE_SOURCES} ${LUA_IMPORTS_SOURCES})
endif()

add_library(ml_uuid SHARED ${FINAL_SOURCES})

if(WIN32)
    target_link_directories(ml_uuid PRIVATE "${CMAKE_SOURCE_DIR}/lib")
    
    target_link_libraries(ml_uuid PRIVATE lua5.1.lib)
    
    target_compile_definitions(ml_uuid PRIVATE 
        WIN32 
        LUA_BUILD_AS_DLL 
        _CRT_SECURE_NO_WARNINGS
    )
else()
    set_target_properties(ml_uuid PROPERTIES 
        PREFIX "" 
        LINK_FLAGS "-Wl,--unresolved-symbols=ignore-all"
    )
    target_link_libraries(ml_uuid PRIVATE dl atomic m)
endif()

target_include_directories(ml_uuid PRIVATE 
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/luaimports"
)

target_link_libraries(ml_uuid PRIVATE Boost::uuid)

set_target_properties(ml_uuid PROPERTIES 
    PREFIX "" 
    OUTPUT_NAME "ml_uuid"
)