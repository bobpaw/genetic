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
