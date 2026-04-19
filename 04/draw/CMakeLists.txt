# logic
add_library(draw_lib
        shapes/ShapeBase.cpp
        shapes/SolidShapeBase.cpp
        shapes/Rectangle.cpp
        shapes/Triangle.cpp
        shapes/LineSegment.cpp
        shapes/Circle.cpp

        parser/ShapeParser.cpp
        analyzer/ShapeAnalyzer.cpp
)

target_include_directories(draw_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

find_package(
        SFML
        COMPONENTS Graphics Window System
        REQUIRED)

# exec
add_executable(draw
        main.cpp
        canvas/SfmlCanvas.cpp
        app/Runner.cpp
)

target_link_libraries(draw
        PRIVATE
        draw_lib
        SFML::Graphics
        SFML::Window
        SFML::System
)

# tests
enable_testing()

add_executable(draw_tests
        test/test_Draw.cpp
)

target_link_libraries(draw_tests
        PRIVATE
        draw_lib
        Catch2::Catch2WithMain
)

target_include_directories(draw_tests PRIVATE
        ${FakeIt_SOURCE_DIR}/single_header/catch
)

include(Catch)
catch_discover_tests(draw_tests)
