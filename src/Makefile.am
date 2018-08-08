# Copyright (C) 2018 Eduardo M KALINOWSKI <eduardo@kalinowski.com.br>.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of the University nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

lib_LTLIBRARIES = libkgtk3-combo-model-1.0.la
libkgtk3_combo_model_1_0_la_SOURCES = kgtk3-combo-model.c
include_HEADERS = kgtk3-combo-model.h
libkgtk3_combo_model_1_0_la_CPPFLAGS = $(GTK_CFLAGS)
libkgtk3_combo_model_1_0_la_LIBADD = $(GTK_LIBS)


-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir) --warn-all
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)

if HAVE_INTROSPECTION
introspection_sources = $(libgtk3_combo_model_1_0_la_SOURCES) $(include_HEADERS)

KGtk3ComboModel-1.0.gir: libkgtk3-combo-model-1.0.la
KGtk3ComboModel_1_0_gir_SCANNERFLAGS = --identifier-prefix=KGtk3 --symbol-prefix=kgtk3
KGtk3ComboModel_1_0_gir_NAMESPACE=KGtk3ComboModel
KGtk3ComboModel_1_0_gir_INCLUDES = Gtk-3.0
KGtk3ComboModel_1_0_gir_CFLAGS = $(INCLUDES)
KGtk3ComboModel_1_0_gir_LIBS = libkgtk3-combo-model-1.0.la
KGtk3ComboModel_1_0_gir_FILES = $(introspection_sources)
INTROSPECTION_GIRS += KGtk3ComboModel-1.0.gir

girdir = @INTROSPECTION_GIRDIR@
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = @INTROSPECTION_TYPELIBDIR@
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES = $(gir_DATA) $(typelib_DATA)
endif

noinst_PROGRAMS = example
example_SOURCES = example.c
example_CPPFLAGS = $(GTK_CFLAGS)
example_LDADD = libkgtk3-combo-model-1.0.la $(GTK_LIBS)
