include_directories(.)

if (NOT HAVE_GETOPT)
add_library(getopt_rpc STATIC rpc/getopt_rpc.c rpc/getopt_rpc.h)
endif()

add_library(gen_alg STATIC gen_alg.cc gen_alg.h)
# target_compile_features(obj1 PUBLIC cxx_auto_type $<IF:$<COMPILE_FEATURES:cxx_return_type_deduction>,cxx_return_type_deduction,cxx_trailing_return_types> cxx_noexcept cxx_nullptr cxx_decltype)
add_library(genetic_string STATIC genetic_string.cc genetic_string.h)
target_link_libraries(genetic_string PUBLIC gen_alg)

add_executable(genetic main.cc main.h correct_curses.h #[[ FIXME: fakerandom.h when valgrind]])
target_include_directories(genetic PRIVATE "${CURSES_INCLUDE_DIRS}")
target_compile_features(genetic PRIVATE cxx_lambdas cxx_decltype)
target_link_libraries(genetic genetic_string gen_alg $<IF:$<BOOL:${HAVE_GETOPT}>,,getopt_rpc> "${CURSES_LIBRARIES}")

add_subdirectory(implementations EXCLUDE_FROM_ALL)

# Tests (enable if this is the top-level project and a test build)
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING) OR GENETIC_BUILD_TESTING)
add_executable(simple_test test/simple_test.cc)
target_link_libraries(simple_test PRIVATE genetic_string)
add_executable(random_inrange test/random_inrange.cc)
add_executable(expand_test test/expand_test.cc)
target_link_libraries(expand_test PRIVATE genetic_string)

add_test(NAME sanity COMMAND "$<TARGET_FILE:genetic>" -V)
add_test(NAME SimpleTest COMMAND "$<TARGET_FILE:simple_test>")
add_test(NAME RandomInRange COMMAND "$<TARGET_FILE:random_inrange>") # FIXME: Rewrite to check that all values are in range
add_test(NAME ExpandTest COMMAND "$<TARGET_FILE:expand_test>")
add_test(NAME IntegratedGenetic COMMAND "$<TARGET_FILE:genetic>" -m14 -p100 -q)
endif()
