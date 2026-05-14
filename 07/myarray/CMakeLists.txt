# logic
add_library(myarray_lib INTERFACE)

target_include_directories(myarray_lib
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/test
)

# exec
add_executable(myarray
        main.cpp
)

target_link_libraries(myarray
        PRIVATE
        myarray_lib
)

# tests
enable_testing()

add_executable(myarray_tests
        test/test_myarray.cpp
)

target_link_libraries(myarray_tests
        PRIVATE
        string_lib
        Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(myarray_tests)
