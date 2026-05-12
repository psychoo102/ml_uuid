file(GLOB SOURCES "*.cpp" "*.c")

add_library(ml_uuid SHARED ${SOURCES})

if(WIN32)
    # Windows musi mieæ .lib, ¿eby wiedzieæ jak wygl¹daj¹ symbole, 
    # nawet jeœli ImportLua je potem podmienia.
    target_link_directories(ml_uuid PRIVATE "${PROJECT_SOURCE_DIR}/lib")
    target_link_libraries(ml_uuid PRIVATE lua5.1)
else()
    # Na Linuxie ignorujemy b³êdy brakuj¹cych symboli podczas linkowania,
    # bo zostan¹ one dostarczone przez serwer MTA (pod warunkiem, ¿e ImportLua dzia³a)
    target_link_libraries(ml_uuid PRIVATE -latomic)
    
    set_target_properties(ml_uuid PROPERTIES 
        PREFIX ""
        LINK_FLAGS "-Wl,--unresolved-symbols=ignore-all"
    )
endif()

target_include_directories(ml_uuid PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(ml_uuid PRIVATE 
    luaimports
    Boost::uuid
)