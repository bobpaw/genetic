add_library(beef STATIC beef.h beef.cc)
add_executable(beef_main beef_main.cc beef.h)
target_link_libraries(beef_main beef)
