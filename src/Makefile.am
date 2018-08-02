lib_LTLIBRARIES = libkgtk3-combo-model-1.0.la
libkgtk3_combo_model_1_0_la_SOURCES = kgtk3-combo-model.c kbutton.c
include_HEADERS = kgtk3-combo-model.h kbutton.h
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

bin_PROGRAMS = test
test_SOURCES = test.c
test_CPPFLAGS = $(GTK_CFLAGS)
test_LDADD = libkgtk3-combo-model-1.0.la $(GTK_LIBS)
