/*
 * Copyright (C) 2018 Eduardo M KALINOWSKI <eduardo@kalinowski.com.br>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <kgtk3-combo-model.h>

#include "test-support.h"


enum
{
  NONE,
  ROW_CHANGED,
};


typedef struct
{
  int          signal;
  GtkTreePath *path;
  GtkTreeIter  iter;
} signal_data;


void on_row_changed(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer user_data)
{
  signal_data *sigdata = (signal_data *) user_data;
  sigdata->signal = ROW_CHANGED;
  sigdata->path = gtk_tree_path_copy(path);
  sigdata->iter = *iter;
}


void check_path(GtkTreePath *path, const char *expected, const char *msg)
{
  gchar *path_str = gtk_tree_path_to_string(path);
  check(strcmp(expected, path_str) == 0, msg);
  g_free(path_str);
}


void create_models(GtkTreeStore **store, GtkTreeModel **model, KGtk3ComboModel **cmodel)
{
  *store = create_model();
  *model = GTK_TREE_MODEL(*store);
  *cmodel = kgtk3_combo_model_new(*model);
}
#define SETUP() signal_data sigdata;   \
                GtkTreeStore *store;     \
                GtkTreeModel *model;     \
                KGtk3ComboModel *cmodel; \
                create_models(&store, &model, &cmodel);


#define CLEANUP() gtk_tree_path_free(sigdata.path);     \
                  g_object_unref(cmodel);               \
                  g_object_unref(store);


void
test_row_changed_root_no_children()
{
  SETUP();
  g_signal_connect(cmodel, "row-changed", G_CALLBACK(on_row_changed), &sigdata);

  GtkTreeIter iter;
  gtk_tree_model_iter_nth_child(model, &iter, NULL, 0);
  gtk_tree_store_set(store, &iter, 0, "New text", -1);

  check(sigdata.signal == ROW_CHANGED, "should emit row_changed - root, no children");
  check_path(sigdata.path, "0", "row_changed path should point to new row - root, no children");

  CLEANUP();
}


int
main(int argc, char *argv[])
{
  test_row_changed_root_no_children();

  /*
   * End
   */
  return finalize();
}
