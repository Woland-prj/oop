# logic
add_library(string_lib
        MyString.cpp
)

target_include_directories(string_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# exec
add_executable(string
        main.cpp
)

target_link_libraries(string
        PRIVATE
        string_lib
)

# tests
enable_testing()

add_executable(string_tests
        test/test_MyString.cpp
)

target_link_libraries(string_tests
        PRIVATE
        string_lib
        Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(string_tests)
