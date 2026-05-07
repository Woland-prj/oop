# logic
add_library(stringlist_lib
    StringList.cpp
)

target_include_directories(stringlist_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# exec
add_executable(stringlist
        main.cpp
)

target_link_libraries(stringlist
        PRIVATE
        stringlist_lib
)

# tests
enable_testing()

add_executable(stringlist_tests
        test/Test_stringlist.cpp
)

target_link_libraries(stringlist_tests
        PRIVATE
        stringlist_lib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(stringlist_tests)
