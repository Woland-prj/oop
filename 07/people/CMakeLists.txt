# logic
add_library(people_lib
 Pupil.cpp
 Teacher.cpp
 Student.cpp
 AdvancedStudent.cpp
 Worker.cpp
)

target_include_directories(people_lib
 PUBLIC
 ${CMAKE_CURRENT_SOURCE_DIR}
)

# exec
add_executable(people
 main.cpp
)

target_link_libraries(people
 PRIVATE
 people_lib
)

# tests
enable_testing()

add_executable(people_tests
 test/Test_people.cpp
)

target_link_libraries(people_tests
 PRIVATE
 people_lib
 Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(people_tests)
