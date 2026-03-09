# logic
add_library(minvec_lib
    NumberProcessing.cpp
)

target_include_directories(minvec_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# exec
add_executable(minvec
    main.cpp
)

target_link_libraries(minvec
    PRIVATE
    minvec_lib
)

# tests
enable_testing()

add_executable(minvec_tests
    tests/test_NumberProcessing.cpp
)

target_link_libraries(minvec_tests
    PRIVATE
    minvec_lib
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(minvec_tests)
