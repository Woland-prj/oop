add_executable(
	replace
	main.cpp
	replacer/StringReplacer.cpp
	stream_processor/StreamIO.cpp
	runner/ReplaceApp.cpp
)

target_include_directories(
    replace PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(replace PROPERTIES OUTPUT_NAME "replace")
