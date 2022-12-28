add_library(beef STATIC beef.h beef.cc)
add_executable(beef_main beef_main.cc beef.h)
target_link_libraries(beef_main beef)

add_library(gen_beef STATIC gen_beef.cc gen_beef.h)
