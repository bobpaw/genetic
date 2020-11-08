add_executable(mancala mancala.cc mancala.h)
target_include_directories(mancala PRIVATE "${CURSES_INCLUDE_DIRS}")
target_compile_features(mancala PRIVATE cxx_lambdas cxx_decltype)
target_link_libraries(mancala "${CURSES_LIBRARIES}")