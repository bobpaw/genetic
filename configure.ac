#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([genetic], [0.1.0], [aiden.woodruff@gmail.com])
AC_CONFIG_SRCDIR([src/gen_alg.cc])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CXXCPP
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX([14], [noext], [mandatory])
if test $HAVE_CXX14 -eq 0; then
AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory])
fi

# Checks for libraries.
AX_WITH_CURSES

# Checks for header files.
AC_CHECK_HEADERS([iostream string numeric algorithm utility vector random functional cstdlib])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
# AC_CHECK_FUNCS([memset strdup strtol])
AC_CHECK_FUNCS([getopt])

AC_CONFIG_FILES([Makefile src/Makefile test/Makefile])

# m4_foreach_w([_script],
# [test/simple.sh test/rand_range.sh test/expand.sh test/sanity test/each_mut.sh test/some_pops.sh],
# [AC_CONFIG_FILES(_script, chmod +x _script)])

AM_INIT_AUTOMAKE
AC_OUTPUT
