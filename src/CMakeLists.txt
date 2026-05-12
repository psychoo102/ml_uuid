file(GLOB SOURCES "*.cpp" "*.c")

add_library(ml_uuid SHARED ${SOURCES})

target_include_directories(ml_uuid PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(ml_uuid PRIVATE 
    lua51
    luaimports
)

# Wa¿ne ustawienia dla modu³u MTA:SA
set_target_properties(ml_uuid PROPERTIES 
    OUTPUT_NAME "ml_uuid"
    PREFIX ""
    SUFFIX ".dll"
)