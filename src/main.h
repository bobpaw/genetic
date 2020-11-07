#include <iostream>
#include <string>
#include <stdexcept>

#include <csignal>
#include <cstdlib>
#include <cerrno>
#include <cctype>
// #include <unistd.h>

#include <curses.h>
#include <term.h>

#include "config.h"
#include "genetic_string.h"

#ifdef HAVE_GETOPT
namespace getopt_uni {
	extern "C" int getopt(int argc, char *const argv[], const char *optstring);
	extern "C" char *optarg;
	extern "C" int optind, opterr, optopt;
}
#endif

