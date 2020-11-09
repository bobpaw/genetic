add_executable(play_mancala play_mancala.cc mancala.h)
target_include_directories(play_mancala PRIVATE "${CURSES_INCLUDE_DIRS}")
target_compile_features(play_mancala PRIVATE cxx_lambdas cxx_decltype)
target_link_libraries(play_mancala "${CURSES_LIBRARIES}")

add_library(gen_manc STATIC gen_manc.cc gen_manc.h mancala.h)
target_link_libraries(gen_manc PUBLIC gen_alg)

add_executable(mancala main.cc main.h)
target_include_directories(mancala PRIVATE "${CURSES_INCLUDE_DIRS}")
target_compile_features(mancala PRIVATE cxx_lambdas cxx_decltype)
target_link_libraries(mancala gen_manc "${CURSES_LIBRARIES}")