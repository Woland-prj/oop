# logic
add_library(exparser_lib
    ExpressionParser.cpp
)

target_include_directories(exparser_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# exec
add_executable(exparser
    main.cpp
)

target_link_libraries(exparser
    PRIVATE
    exparser_lib
)

# tests
enable_testing()

add_executable(exparser_tests
    test/test_EvalPrefixExpression.cpp
)

target_link_libraries(exparser_tests
    PRIVATE
    exparser_lib
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(exparser_tests)
