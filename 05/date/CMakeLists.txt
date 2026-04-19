# logic
add_library(date_lib
        Date.cpp
)

target_include_directories(date_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# exec
add_executable(date
        main.cpp
)

target_link_libraries(date
        PRIVATE
        date_lib
)

# tests
enable_testing()

add_executable(date_tests
        test/test_Date.cpp
)

target_link_libraries(date_tests
        PRIVATE
        date_lib
        Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(date_tests)
