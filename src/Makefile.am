lib_LTLIBRARIES = libkgtk3-combo-model.la
libkgtk3_combo_model_la_SOURCES = kbutton.c
include_HEADERS = kbutton.h
libkgtk3_combo_model_la_CPPFLAGS = $(GTK_CFLAGS)

bin_PROGRAMS = test
test_SOURCES = test.c
test_CPPFLAGS = $(GTK_CFLAGS)
test_LDADD = libkgtk3-combo-model.la $(GTK_LIBS)
