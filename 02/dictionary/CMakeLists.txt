# logic
add_library(dictionary_lib
    Dictionary.hpp
    Dictionary.cpp
    WordTransform.hpp
    WordTransform.cpp
)

target_include_directories(dictionary_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# exec
add_executable(dictionary
    main.cpp
    DictionaryApp.hpp
    DictionaryApp.cpp
)

target_link_libraries(dictionary
    PRIVATE
    dictionary_lib
)

# tests
enable_testing()

add_executable(dictionary_tests
    test/test_Dictionary.cpp
)

target_link_libraries(dictionary_tests
    PRIVATE
    dictionary_lib
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(dictionary_tests)
