# logic
add_library(html_decode_lib
    HtmlDecoding.cpp
)

target_include_directories(html_decode_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# exec
add_executable(html_decode
    main.cpp
)

target_link_libraries(html_decode
    PRIVATE
    html_decode_lib
)

# tests
enable_testing()

add_executable(html_decode_tests
    tests/test_HtmlDecode.cpp
)

target_link_libraries(html_decode_tests
    PRIVATE
    html_decode_lib
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(html_decode_tests)
