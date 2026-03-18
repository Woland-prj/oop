# logic
add_library(regexp_lib
    URLParser.hpp
    URLParser.cpp
)

target_include_directories(regexp_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# exec
add_executable(regexp
    RegexpApp.hpp
    RegexpApp.cpp
    main.cpp
)

target_link_libraries(regexp
    PRIVATE
    regexp_lib
)

# tests
enable_testing()

add_executable(regexp_tests
    test/test_ParseURL.cpp
)

target_link_libraries(regexp_tests
    PRIVATE
    regexp_lib
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(regexp_tests)
