# logic
add_library(httpurl_lib
        HttpUrl.cpp
        UrlParsingError.cpp
        Utf8Validation.cpp
)

target_include_directories(httpurl_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)
# find_package(Boost REQUIRED COMPONENTS stacktrace MODULE)


# exec
add_executable(httpurl
        main.cpp
)

target_link_libraries(httpurl
        PRIVATE
        httpurl_lib
)

# tests
enable_testing()

add_executable(httpurl_tests
        test/test_HttpUrl.cpp
        test/test_Utf8Validation.cpp
)

target_link_libraries(httpurl_tests
        PRIVATE
        httpurl_lib
        Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(httpurl_tests)
