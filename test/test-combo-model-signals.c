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
  ROW_INSERTED,
  ROW_DELETED,
  ROW_HAS_CHILD_TOGGLED,
};


typedef struct
{
  int          signal;
  GtkTreePath *path;
  GtkTreeIter  iter;
} signal_data;

void free_signal_data(signal_data *sigdata)
{
  gtk_tree_path_free(sigdata->path);
  g_free(sigdata);
}


void on_row_changed(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer user_data)
{
  GSList **sigdatas = (GSList **) user_data;

  signal_data *sigdata = g_new0(signal_data, 1);
  sigdata->signal = ROW_CHANGED;
  sigdata->path = gtk_tree_path_copy(path);
  sigdata->iter = *iter;

  *sigdatas = g_slist_append(*sigdatas, sigdata);
}


void on_row_inserted(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer user_data)
{
  GSList **sigdatas = (GSList **) user_data;

  signal_data *sigdata = g_new0(signal_data, 1);
  sigdata->signal = ROW_INSERTED;
  sigdata->path = gtk_tree_path_copy(path);
  sigdata->iter = *iter;

  *sigdatas = g_slist_append(*sigdatas, sigdata);
}


void on_row_deleted(GtkTreeModel *model, GtkTreePath *path, gpointer user_data)
{
  GSList **sigdatas = (GSList **) user_data;

  signal_data *sigdata = g_new0(signal_data, 1);
  sigdata->signal = ROW_DELETED;
  sigdata->path = gtk_tree_path_copy(path);

  *sigdatas = g_slist_append(*sigdatas, sigdata);
}


void on_row_has_child_toggled(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer user_data)
{
  GSList **sigdatas = (GSList **) user_data;

  signal_data *sigdata = g_new0(signal_data, 1);
  sigdata->signal = ROW_HAS_CHILD_TOGGLED;
  sigdata->path = gtk_tree_path_copy(path);
  sigdata->iter = *iter;

  *sigdatas = g_slist_append(*sigdatas, sigdata);
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
#define SETUP() GSList       *sigdatas = NULL; \
                GtkTreeStore *store;           \
                GtkTreeModel *model;           \
                KGtk3ComboModel *cmodel;       \
                create_models(&store, &model, &cmodel);


#define CLEANUP() g_slist_free_full(sigdatas, (GDestroyNotify) free_signal_data); \
                  g_object_unref(cmodel); \
                  g_object_unref(store);


void
test_row_changed_root_no_children()
{
  SETUP();
  g_signal_connect(cmodel, "row-changed",
                   G_CALLBACK(on_row_changed), &sigdatas);

  GtkTreeIter iter;
  gtk_tree_model_iter_nth_child(model, &iter, NULL, 0);
  gtk_tree_store_set(store, &iter, 0, "New text", -1);

  signal_data *sigdata = sigdatas->data;
  check(sigdata->signal == ROW_CHANGED,
        "should emit row_changed - root, no children");
  check_path(sigdata->path, "0",
             "row_changed path points to changed row - root, no children");
  check(sigdatas->next == NULL,
        "only one row_changed emitted - root, no children");

  CLEANUP();
}


void
test_row_changed_level1_no_children()
{
  SETUP();
  g_signal_connect(cmodel, "row-changed",
                   G_CALLBACK(on_row_changed), &sigdatas);

  GtkTreeIter top_level, level1;
  gtk_tree_model_iter_nth_child(model, &top_level, NULL, 1);
  gtk_tree_model_iter_nth_child(model, &level1, &top_level, 2);
  gtk_tree_store_set(store, &level1, 0, "New text", -1);

  signal_data *sigdata = sigdatas->data;
  check(sigdata->signal == ROW_CHANGED,
        "should emit row_changed - level1, no children");
  check_path(sigdata->path, "1:4",
             "row_changed path points to changed row - level1, no children");
  check(sigdatas->next == NULL,
        "only one row_changed emitted - level1, no children");

  CLEANUP();
}


void
test_row_changed_root_with_children()
{
  SETUP();
  g_signal_connect(cmodel, "row-changed",
                   G_CALLBACK(on_row_changed), &sigdatas);

  GtkTreeIter iter;
  gtk_tree_model_iter_nth_child(model, &iter, NULL, 1);
  gtk_tree_store_set(store, &iter, 0, "New text", -1);

  GSList *i = sigdatas;
  signal_data *sigdata = i->data;
  check(sigdata->signal == ROW_CHANGED,
        "should emit row_changed - root, with children, regular");
  check_path(sigdata->path, "1",
             "row_changed path points to changed row - root, with children, regular");

  i = i->next;
  sigdata = i->data;
  check(sigdata->signal == ROW_CHANGED,
        "should emit row_changed - root, with children, header");
  check_path(sigdata->path, "1:0",
             "row_changed path points to changed row - root, with children, header");

  check(i->next == NULL,
        "two row_changed emitted - root, with children");

  CLEANUP();
}


void
test_row_changed_level1_with_children()
{
  SETUP();
  g_signal_connect(cmodel, "row-changed",
                   G_CALLBACK(on_row_changed), &sigdatas);

  GtkTreeIter top_level, level1;
  gtk_tree_model_iter_nth_child(model, &top_level, NULL, 2);
  gtk_tree_model_iter_nth_child(model, &level1, &top_level, 0);
  gtk_tree_store_set(store, &level1, 0, "New text", -1);

  GSList *i = sigdatas;
  signal_data *sigdata = i->data;
  check(sigdata->signal == ROW_CHANGED,
        "should emit row_changed - level1, with children, regular");
  check_path(sigdata->path, "2:2",
             "row_changed path points to changed row - level1, with children, regular");

  i = i->next;
  sigdata = i->data;
  check(sigdata->signal == ROW_CHANGED,
        "should emit row_changed - level1, with children, header");
  check_path(sigdata->path, "2:2:0",
             "row_changed path points to changed row - level1, with children, header");

  check(i->next == NULL,
        "two row_changed emitted - level1, with children");

  CLEANUP();
}


void
test_row_inserted_root()
{
  SETUP();
  g_signal_connect(cmodel, "row-inserted",
                   G_CALLBACK(on_row_inserted), &sigdatas);

  GtkTreeIter iter;
  gtk_tree_store_append(store, &iter, NULL);

  signal_data *sigdata = sigdatas->data;
  check(sigdata->signal == ROW_INSERTED,
        "should emit row_inserted - root");
  check_path(sigdata->path, "3",
             "row_inserted path points to inserted row - root");
  check(sigdatas->next == NULL,
        "only one row_inserted emitted - root");

  CLEANUP();
}


void
test_row_inserted_level2()
{
  SETUP();
  g_signal_connect(cmodel, "row-inserted",
                   G_CALLBACK(on_row_inserted), &sigdatas);

  GtkTreeIter top_level, level1, level2;
  gtk_tree_model_iter_nth_child(model, &top_level, NULL, 2);
  gtk_tree_model_iter_nth_child(model, &level1, &top_level, 0);
  gtk_tree_store_insert(store, &level2, &level1, 0);

  signal_data *sigdata = sigdatas->data;
  check(sigdata->signal == ROW_INSERTED,
        "should emit row_inserted - level 2");
  check_path(sigdata->path, "2:2:2",
             "row_inserted path points to inserted row - level 2");
  check(sigdatas->next == NULL,
        "only one row_inserted emitted - level 2");

  CLEANUP();
}


void
test_row_inserted_including_virtual_items()
{
  SETUP();
  g_signal_connect(cmodel, "row-inserted",
                   G_CALLBACK(on_row_inserted), &sigdatas);

  GtkTreeIter iter, level1;
  gtk_tree_model_iter_children(model, &iter, NULL);
  gtk_tree_store_insert(store, &level1, &iter, 0);

  GSList *i = sigdatas;
  signal_data *sigdata = i->data;
  check(sigdata->signal == ROW_INSERTED,
        "should emit row_inserted for virtual items - header");
  check_path(sigdata->path, "0:0",
             "should emit row_inserted for virtual items - header path");

  i = i->next;
  sigdata = i->data;
  check(sigdata->signal == ROW_INSERTED,
        "should emit row_inserted for virtual items - separator");
  check_path(sigdata->path, "0:1",
             "should emit row_inserted for virtual items - separator path");

  i = i->next;
  sigdata = i->data;
  check(sigdata->signal == ROW_INSERTED,
        "should emit row_inserted for virtual items - regular");
  check_path(sigdata->path, "0:2",
             "should emit row_inserted for virtual items - regular path");

  check(i->next == NULL,
        "three row_inserted emitted for virtual items");

  CLEANUP();
}


void
test_row_deleted_root()
{
  SETUP();
  g_signal_connect(cmodel, "row-deleted",
                   G_CALLBACK(on_row_deleted), &sigdatas);

  GtkTreeIter iter;
  gtk_tree_model_iter_nth_child(model, &iter, NULL, 0);
  gtk_tree_store_remove(store, &iter);

  signal_data *sigdata = sigdatas->data;
  check(sigdata->signal == ROW_DELETED,
        "should emit row_deleted - root");
  check_path(sigdata->path, "0",
             "row_deleted path points to deleted row - root");
  check(sigdatas->next == NULL,
        "only one row_deleted emitted - root");

  CLEANUP();
}


void
test_row_deleted_level1()
{
  SETUP();
  g_signal_connect(cmodel, "row-deleted",
                   G_CALLBACK(on_row_deleted), &sigdatas);

  GtkTreeIter top_level, level1;
  gtk_tree_model_iter_nth_child(model, &top_level, NULL, 2);
  gtk_tree_model_iter_nth_child(model, &level1, &top_level, 0);
  gtk_tree_store_remove(store, &level1);

  signal_data *sigdata = sigdatas->data;
  check(sigdata->signal == ROW_DELETED,
        "should emit row_deleted - level1");
  check_path(sigdata->path, "2:2",
             "row_deleted path points to deleted row - level1");
  check(sigdatas->next == NULL,
        "only one row_deleted emitted - level1");

  CLEANUP();
}


void
test_row_deleted_including_virtual_items()
{
  SETUP();
  g_signal_connect(cmodel, "row-deleted",
                   G_CALLBACK(on_row_deleted), &sigdatas);

  GtkTreeIter top_level, level1, level2;
  gtk_tree_model_iter_nth_child(model, &top_level, NULL, 2);
  gtk_tree_model_iter_nth_child(model, &level1, &top_level, 0);
  gtk_tree_model_iter_nth_child(model, &level2, &level1, 0);
  gtk_tree_store_remove(store, &level2);

  GSList *i = sigdatas;
  signal_data *sigdata = i->data;
  check(sigdata->signal == ROW_DELETED,
        "should emit row_deleted for regular item");
  check_path(sigdata->path, "2:2:2",
             "should emit row_deleted for regular item - path");

  i = i->next;
  sigdata = i->data;
  check(sigdata->signal == ROW_DELETED,
        "should emit row_deleted for separator item");
  check_path(sigdata->path, "2:2:1",
             "should emit row_deleted for separator item - path");

  i = i->next;
  sigdata = i->data;
  check(sigdata->signal == ROW_DELETED,
        "should emit row_deleted for header item");
  check_path(sigdata->path, "2:2:0",
             "should emit row_deleted for header item - path");

  check(i->next == NULL,
        "three row_deleted emitted - removing last child");

  CLEANUP();
}


void
test_row_has_child_toggled_on_deletion()
{
  SETUP();
  g_signal_connect(cmodel, "row-has-child-toggled",
                   G_CALLBACK(on_row_has_child_toggled), &sigdatas);

  GtkTreeIter top_level, level1, level2;
  gtk_tree_model_iter_nth_child(model, &top_level, NULL, 2);
  gtk_tree_model_iter_nth_child(model, &level1, &top_level, 0);
  gtk_tree_model_iter_nth_child(model, &level2, &level1, 0);
  gtk_tree_store_remove(store, &level2);

  signal_data *sigdata = sigdatas->data;
  check(sigdata->signal == ROW_HAS_CHILD_TOGGLED,
        "should emit row_has_child_toggled when last child is deleted");
  check_path(sigdata->path, "2:2",
             "should emit row_has_child_toggled when last child is deleted - path");

  check(sigdatas->next == NULL,
        "only one row_has_child_toggled emitted when last child is deleted");

  CLEANUP();
}


void
test_signal_order_when_deleting_last_child()
{
  SETUP();
  g_signal_connect(cmodel, "row-deleted",
                   G_CALLBACK(on_row_deleted), &sigdatas);
  g_signal_connect(cmodel, "row-has-child-toggled",
                   G_CALLBACK(on_row_has_child_toggled), &sigdatas);

  GtkTreeIter top_level, level1, level2;
  gtk_tree_model_iter_nth_child(model, &top_level, NULL, 2);
  gtk_tree_model_iter_nth_child(model, &level1, &top_level, 0);
  gtk_tree_model_iter_nth_child(model, &level2, &level1, 0);
  gtk_tree_store_remove(store, &level2);

  GSList *i = sigdatas;
  signal_data *sigdata = i->data;
  check(sigdata->signal == ROW_DELETED,
        "signals when deleting last child - 1st should be row_deleted");

  i = i->next;
  sigdata = i->data;
  check(sigdata->signal == ROW_DELETED,
        "signals when deleting last child - 2nd should be row_deleted");

  i = i->next;
  sigdata = i->data;
  check(sigdata->signal == ROW_DELETED,
        "signals when deleting last child - 3rd should be row_deleted");

  i = i->next;
  sigdata = i->data;
  check(sigdata->signal == ROW_HAS_CHILD_TOGGLED,
        "signals when deleting last child - 4th should be row_has_child_toggled");

  check(i->next == NULL,
        "signals when deleting last child - 4 signals emitted");

  CLEANUP();
}


void
test_row_has_child_toggled_on_insertion()
{
  SETUP();
  g_signal_connect(cmodel, "row-has-child-toggled",
                   G_CALLBACK(on_row_has_child_toggled), &sigdatas);

  GtkTreeIter top_level, level1, level2;
  gtk_tree_model_iter_nth_child(model, &top_level, NULL, 1);
  gtk_tree_model_iter_nth_child(model, &level1, &top_level, 1);
  gtk_tree_store_append(store, &level2, &level1);

  signal_data *sigdata = sigdatas->data;
  check(sigdata->signal == ROW_HAS_CHILD_TOGGLED,
        "should emit row_has_child_toggled when first child is inserted");
  check_path(sigdata->path, "1:3",
             "should emit row_has_child_toggled when first child is inserted - path");

  check(sigdatas->next == NULL,
        "only one row_has_child_toggled emitted when first child is inserted");

  CLEANUP();
}


void
test_signal_order_when_inserting_first_child()
{
  SETUP();
  g_signal_connect(cmodel, "row-inserted",
                   G_CALLBACK(on_row_inserted), &sigdatas);
  g_signal_connect(cmodel, "row-has-child-toggled",
                   G_CALLBACK(on_row_has_child_toggled), &sigdatas);

  GtkTreeIter iter, level1;
  gtk_tree_model_iter_children(model, &iter, NULL);
  gtk_tree_store_insert(store, &level1, &iter, 0);

  GSList *i = sigdatas;
  signal_data *sigdata = i->data;
  check(sigdata->signal == ROW_INSERTED,
        "signals when inserting first child - 1st should be row_inserted");

  i = i->next;
  sigdata = i->data;
  check(sigdata->signal == ROW_HAS_CHILD_TOGGLED,
        "signals when inserting first child - 2nd should be row_has_child_toggled");

  i = i->next;
  sigdata = i->data;
  check(sigdata->signal == ROW_INSERTED,
        "signals when inserting first child - 3rd should be row_inserted");

  i = i->next;
  sigdata = i->data;
  check(sigdata->signal == ROW_INSERTED,
        "signals when inserting first child - 4th should be row_inserted");

  check(i->next == NULL,
        "signals when inserting first child - 4 signals emitted");

  CLEANUP();
}


int
main(int argc, char *argv[])
{
  test_row_changed_root_no_children();
  test_row_changed_level1_no_children();
  test_row_changed_root_with_children();
  test_row_changed_level1_with_children();

  test_row_inserted_root();
  test_row_inserted_level2();
  test_row_inserted_including_virtual_items();

  test_row_deleted_root();
  test_row_deleted_level1();
  test_row_deleted_including_virtual_items();

  test_row_has_child_toggled_on_deletion();
  test_signal_order_when_deleting_last_child();
  test_row_has_child_toggled_on_insertion();
  test_signal_order_when_inserting_first_child();

  /*
   * End
   */
  return finalize();
}
