# Test targets

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(simple_test EXCLUDE_FROM_ALL simple_test.cc)
target_link_libraries(simple_test PRIVATE genetic_string)
add_executable(random_inrange EXCLUDE_FROM_ALL random_inrange.cc ${PROJECT_SOURCE_DIR}/src/rpc/fakerandom.h)
add_executable(expand_test EXCLUDE_FROM_ALL expand_test.cc)
target_link_libraries(expand_test PRIVATE genetic_string)

add_test(
	NAME
		sanity
	COMMAND
		"${CMAKE_CTEST_COMMAND}"
			--output-on-failure
			--build-and-test "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
			--build-generator "${CMAKE_GENERATOR}"
			--build-nocmake
			--build-target genetic
			--test-command "$<TARGET_FILE:genetic>" -V
)

add_test(
	NAME
		SimpleTest
	COMMAND
		"${CMAKE_CTEST_COMMAND}"
			--output-on-failure
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
			--output-on-failure
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
			--output-on-failure
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
			--output-on-failure
			--build-and-test "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
			--build-generator "${CMAKE_GENERATOR}"
			--build-nocmake
			--build-target genetic
			--test-command "$<TARGET_FILE:genetic>" -m14 -p100 -q
)

# FIXME: test each_mut, fixed by adding Catch2
# FIXME: test some_pops, fixed by adding Catch2
