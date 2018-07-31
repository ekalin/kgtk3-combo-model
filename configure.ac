AC_INIT([combo-header-model], [1.0.0])
AC_CONFIG_SRCDIR([src/kbutton.c])

AM_INIT_AUTOMAKE([foreign dist-xz no-dist-gzip])
AM_SILENT_RULES([yes])

AC_PROG_CC

PKG_CHECK_MODULES([GTK], [gtk+-3.0])
AC_SUBST([GTK_CFLAGS])
AC_SUBST([GTK_LIBS])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
