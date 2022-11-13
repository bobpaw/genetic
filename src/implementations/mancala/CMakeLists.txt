add_library(mancala STATIC mancala.cc mancala.h)

add_library(gen_manc STATIC gen_manc.cc gen_manc.h)
target_link_libraries(gen_manc PUBLIC gen_alg mancala)

add_executable(play_mancala play_mancala.cc)
target_include_directories(play_mancala PRIVATE "${CURSES_INCLUDE_DIRS}")
target_compile_features(play_mancala PRIVATE cxx_lambdas cxx_decltype)
target_link_libraries(play_mancala mancala "${CURSES_LIBRARIES}")

add_executable(genetic_mancala main.cc main.h)
target_include_directories(genetic_mancala PRIVATE "${CURSES_INCLUDE_DIRS}")
target_compile_features(genetic_mancala PRIVATE cxx_lambdas cxx_decltype)
target_link_libraries(genetic_mancala gen_manc "${CURSES_LIBRARIES}")