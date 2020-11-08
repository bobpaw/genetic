#include <iostream>
#include <stdexcept>
#include <string>

#include <cctype>
#include <cerrno>
#include <csignal>
#include <cstdlib>
// #include <unistd.h>

#include <curses.h>
#include <term.h>

#include "config.h"
#include "genetic_string.h"

#ifdef HAVE_GETOPT
namespace getopt_uni {
extern "C" int getopt(int argc, char* const argv[], const char* optstring);
extern "C" char* optarg;
extern "C" int optind, opterr, optopt;
}  // namespace getopt_uni
#else
namespace getopt_uni {
#include "getopt_rpc.h"
}
#endif
