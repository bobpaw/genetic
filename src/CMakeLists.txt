include_directories(.)
add_library(gen_alg STATIC gen_alg.cc gen_alg.h)
# target_compile_features(obj1 PUBLIC cxx_auto_type $<IF:$<COMPILE_FEATURES:cxx_return_type_deduction>,cxx_return_type_deduction,cxx_trailing_return_types> cxx_noexcept cxx_nullptr cxx_decltype)
add_library(genetic_string STATIC genetic_string.cc genetic_string.h)
target_link_libraries(genetic_string PUBLIC gen_alg)

add_executable(genetic main.cc main.h correct_curses.h #[[ FIXME: fakerandom.h when valgrind]])
target_include_directories(genetic PRIVATE "${CURSES_INCLUDE_DIRS}")
target_compile_features(genetic PRIVATE cxx_lambdas cxx_decltype)
target_link_libraries(genetic genetic_string gen_alg "${CURSES_LIBRARIES}")

add_executable(mancala mancala.cc mancala.h)
target_include_directories(mancala PRIVATE "${CURSES_INCLUDE_DIRS}")
target_compile_features(mancala PRIVATE cxx_lambdas cxx_decltype)
target_link_libraries(mancala "${CURSES_LIBRARIES}")

# Tests
add_executable(simple_test EXCLUDE_FROM_ALL simple_test.cc genetic_string)
add_executable(random_inrange EXCLUDE_FROM_ALL random_inrange.cc fakerandom.h)
add_executable(expand_test EXCLUDE_FROM_ALL expand_test.cc genetic_string)