# This executable compiles the production callbacks, with the Lua stack
# functions replaced by test doubles in generator_tests.cpp.
add_executable(ml_uuid_generator_tests generator_tests.cpp)
target_include_directories(ml_uuid_generator_tests PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/luaimports"
)
target_link_libraries(ml_uuid_generator_tests PRIVATE Boost::uuid Threads::Threads)

# Match the static vcpkg Boost triplets on Windows.
if(MSVC)
    set_property(TARGET ml_uuid_generator_tests PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_test(NAME ml_uuid.generator_callbacks COMMAND ml_uuid_generator_tests)
