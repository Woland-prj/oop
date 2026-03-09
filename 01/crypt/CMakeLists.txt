add_executable(
    crypt
    main.cpp
    Crypting.cpp
    CryptApp.cpp
)

target_include_directories(
    crypt PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(crypt PROPERTIES OUTPUT_NAME "crypt")
