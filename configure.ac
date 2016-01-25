AC_INIT(./gamesetmatch.c)

PACKAGE=gamesetmatch
VERSION=0.1.1


AM_INIT_AUTOMAKE($PACKAGE, $VERSION)
AC_PROG_RANLIB
AC_CHECK_LIB([m],[cos])
AC_CHECK_LIB([gslcblas],[cblas_dgemm],[],[AC_MSG_ERROR([gslcblas not installed])])
AC_CHECK_LIB([gsl],[gsl_blas_dgemm],[],[AC_MSG_ERROR([gsl not installed])])
AC_CHECK_HEADERS([gsl/gsl_randist.h gsl/gsl_cdf.h math.h getopt.h])

_AC_SRCDIRS(["."])

AC_PROG_CC
AC_SUBST([INTL_LIBS])
AC_OUTPUT(Makefile)
