AC_INIT([combo-header-model], [1.0.0])
AC_CONFIG_SRCDIR([src/kbutton.c])

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign dist-xz no-dist-gzip])
AM_SILENT_RULES([yes])

AC_PROG_CC

LT_INIT
AC_SUBST([LIBTOOL_DEPS])

PKG_CHECK_MODULES([GTK], [gtk+-3.0])
AC_SUBST([GTK_CFLAGS])
AC_SUBST([GTK_LIBS])

GOBJECT_INTROSPECTION_CHECK([1.40.0])
if test "x$found_introspection" = "xyes"; then
   INTROSPECTION_GIRDIR=`$PKG_CONFIG --define-variable=prefix='${prefix}' --variable=girdir gobject-introspection-1.0`
   AC_SUBST(INTROSPECTION_GIRDIR)

   INTROSPECTION_TYPELIBDIR=`$PKG_CONFIG --define-variable=prefix='${prefix}' --variable=typelibdir gobject-introspection-1.0`
   AC_SUBST(INTROSPECTION_TYPELIBDIR)
fi

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
