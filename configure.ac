#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([cstl], [0.0.1], [ryuuzaki.uchiha@gmail.com])
AM_INIT_AUTOMAKE
LT_INIT
AC_CONFIG_SRCDIR([include/common.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
#AM_PROG_AR
AC_PROG_CC
AC_PROG_LIBTOOL
AM_PROG_CC_C_O

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_ARG_ENABLE([debug],
              [--enable-debug turn on debugging],
              [case "${enableval}" in
               yes) debug=true;;
               no)  debug=false;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]);;
               esac],
              [debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile
                 test/Makefile])
AC_OUTPUT
