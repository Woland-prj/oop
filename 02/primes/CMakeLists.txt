# logic
add_library(primes_lib
    Primes.hpp
    Primes.cpp
)

target_include_directories(primes_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# exec
add_executable(primes
    main.cpp
)

target_link_libraries(primes
    PRIVATE
    primes_lib
)

# tests
enable_testing()

add_executable(primes_tests
    test/test_Primes.cpp
)

target_link_libraries(primes_tests
    PRIVATE
    primes_lib
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(primes_tests)
