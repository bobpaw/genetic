bin_PROGRAMS = genetic mancala
noinst_HEADERS = gen_alg.h fakerandom.h
genetic_SOURCES = main.cc main.h gen_alg.cc genetic_string.cc
genetic_LDADD = $(CURSES_LIBS)
genetic_CXXFLAGS = $(CURSES_CFLAGS)
check_PROGRAMS = simple_test random_inrange expand_test
simple_test_SOURCES = simple_test.cc gen_alg.cc
random_inrange_SOURCES = random_inrange.cc
expand_test_SOURCES = expand_test.cc gen_alg.cc

mancala_SOURCES = mancala.cc mancala.h
mancala_LDADD = $(CURSES_LIBS)
mancala_CXXFLAGS = $(CURSESIC_FLAGS)