add_executable(
	flipbyte
	main.cpp
)

target_include_directories(
    flipbyte PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(replace PROPERTIES OUTPUT_NAME "flipbyte")
