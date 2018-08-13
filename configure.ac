dnl Copyright (C) 2018 Eduardo M KALINOWSKI <eduardo@kalinowski.com.br>.
dnl All rights reserved.
dnl
dnl Redistribution and use in source and binary forms, with or without
dnl modification, are permitted provided that the following conditions
dnl are met:
dnl 1. Redistributions of source code must retain the above copyright
dnl    notice, this list of conditions and the following disclaimer.
dnl 2. Redistributions in binary form must reproduce the above copyright
dnl    notice, this list of conditions and the following disclaimer in the
dnl    documentation and/or other materials provided with the distribution.
dnl 3. Neither the name of the University nor the names of its contributors
dnl    may be used to endorse or promote products derived from this software
dnl    without specific prior written permission.
dnl
dnl THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
dnl ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
dnl IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
dnl ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
dnl FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
dnl DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
dnl OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
dnl HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
dnl LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
dnl OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
dnl SUCH DAMAGE.

AC_INIT([kgtk3-combo-model], [1.1.0])
AC_CONFIG_SRCDIR([src/kgtk3-combo-model.c])

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign dist-xz no-dist-gzip])
AM_SILENT_RULES([yes])

AC_PROG_CC

LT_INIT
AC_SUBST([LIBTOOL_DEPS])

# Libtool versioning - only update before a public release.
# If the code has changed at all: REVISION++.
# If any interfaces have changed at all: CURRENT++, REVISION=0.
# If any interfaces have been added since the last public release, AGE++
# If any interfaces have been removed or changed since the last public release, AGE=0.
CURRENT=1
AGE=1
REVISION=0
LT_VERSION_INFO="$CURRENT:$REVISION:$AGE"
AC_SUBST(LT_VERSION_INFO)

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
                 src/Makefile
                 test/Makefile
                 src/kgtk3-combo-model-1.0.pc])
AC_OUTPUT
