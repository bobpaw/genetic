add_test(
	NAME
		sanity
	COMMAND
		"${CMAKE_CTEST_COMMAND}"
			--build-and-test "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
			--build-generator "${CMAKE_GENERATOR}"
			--build-nocmake
			--build-target genetic
			--test-command "$<TARGET_FILE:genetic> -V"
)

add_test(
	NAME
		SimpleTest
	COMMAND
		"${CMAKE_CTEST_COMMAND}"
			--build-and-test "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
			--build-generator "${CMAKE_GENERATOR}"
			--build-nocmake
			--build-target simple_test
			--test-command "$<TARGET_FILE:simple_test>"
)

add_test(
	NAME
		RandomInRange
	COMMAND
		"${CMAKE_CTEST_COMMAND}"
			--build-and-test "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
			--build-generator "${CMAKE_GENERATOR}"
			--build-nocmake
			--build-target random_inrange
			--test-command "$<TARGET_FILE:random_inrange>"
			# FIXME: Rewrite to check that all values are in range
)

add_test(
	NAME
		ExpandTest
	COMMAND
		"${CMAKE_CTEST_COMMAND}"
			--build-and-test "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
			--build-generator "${CMAKE_GENERATOR}"
			--build-nocmake
			--build-target expand_test
			--test-command "$<TARGET_FILE:expand_test>"
)

add_test(
	NAME
		IntegratedGenetic
	COMMAND
		"${CMAKE_CTEST_COMMAND}"
			--build-and-test "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
			--build-generator "${CMAKE_GENERATOR}"
			--build-nocmake
			--build-target genetic
			--test-command "$<TARGET_FILE:genetic> -m14 -p100 -q"
)

# FIXME: test each_mut, fixed by adding Catch2
# FIXME: test some_pops, fixed by adding Catch2
