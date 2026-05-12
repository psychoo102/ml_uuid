file(GLOB SOURCES "*.cpp" "*.c")

add_library(ml_uuid SHARED ${SOURCES})

target_include_directories(ml_uuid PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(ml_uuid PRIVATE 
    lua51
    luaimports
)