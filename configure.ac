#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([genetic], [0.1.0], [aiden.woodruff@gmail.com])
AC_CONFIG_SRCDIR([src/gen_alg.cc])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CXXCPP
AC_LANG([C++])

# Checks for libraries.
AC_CHECK_LIB([ncurses], [initscr])
AC_CHECK_LIB([m], [fabs])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h time.h stdio.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset strdup strtol])

AC_CONFIG_FILES([Makefile src/Makefile test/Makefile test/simple.sh], [chmod +x test/simple.sh])
AM_INIT_AUTOMAKE
AC_OUTPUT
