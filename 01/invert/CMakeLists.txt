add_executable(
	invert
	main.cpp
	runner/Runner.cpp
	matrix/Matrix.tpp
	matrix/Matrix.hpp
	inverting/Inverting.hpp
)

target_include_directories(
    invert PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(replace PROPERTIES OUTPUT_NAME "invert")
