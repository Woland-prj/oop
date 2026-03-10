add_executable(
    fill
    main.cpp
    Image.cpp
    Filling.cpp
    FillApp.cpp
)

target_include_directories(
    fill PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(crypt PROPERTIES OUTPUT_NAME "fill")
