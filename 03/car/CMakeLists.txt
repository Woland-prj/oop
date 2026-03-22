# logic
add_library(car_lib
    Car.hpp
    Car.cpp
    CarDriver.hpp
    CarDriver.cpp
)

target_include_directories(car_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# exec
add_executable(car
    main.cpp
)

target_link_libraries(car
    PRIVATE
    car_lib
)

# tests
enable_testing()

add_executable(car_tests
    test/test_Car.cpp
)

target_link_libraries(car_tests
    PRIVATE
    car_lib
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(car_tests)
