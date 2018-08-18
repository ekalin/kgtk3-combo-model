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


void check_col_int(GtkTreeModel *model, GtkTreeIter *iter, gint expected, const char *msg)
{
  gint value;
  gtk_tree_model_get(model, iter, 0, &value, -1);
  check(expected == value, msg);
}

void check_path(GtkTreeModel *model, GtkTreeIter *iter, const char *expected, const char *msg)
{
  GtkTreePath *path = gtk_tree_model_get_path(model, iter);
  if (path == NULL) {
    printf("FAILED\n");
    ++errors;
    return;
  }

  gchar *path_str = gtk_tree_path_to_string(path);
  check(strcmp(expected, path_str) == 0, msg);
  gtk_tree_path_free(path);
  g_free(path_str);
}


int
main(int argc, char *argv[])
{
  /*
   * Set up
   */
  GtkTreeIter top_level, child, subchild, parent;
  GtkTreePath *path;

  gtk_init(&argc, &argv);

  GtkTreeStore *model_store = create_model();
  GtkTreeModel *model = GTK_TREE_MODEL(model_store);
  KGtk3ComboModel *cmodel_k = kgtk3_combo_model_new(model);
  GtkTreeModel *cmodel = GTK_TREE_MODEL(cmodel_k);


  /*
   * Construction
   */
  check(kgtk3_combo_model_new(NULL) == NULL,
        "should fail when the constructor receives NULL");

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  check(kgtk3_combo_model_new((GtkTreeModel *) window) == NULL,
        "should fail when the constructor receives an invalid object");


  /*
   * Flags
   */
  check(gtk_tree_model_get_flags(cmodel) == gtk_tree_model_get_flags(model),
        "should return the same flags");


  /*
   * Columns
   */
  check(gtk_tree_model_get_n_columns(cmodel)
        == gtk_tree_model_get_n_columns(model) + 1,
        "should return one more column");
  check(gtk_tree_model_get_column_type(cmodel, 0)
        == gtk_tree_model_get_column_type(model, 0),
        "should return existing column type (column 0)");
  check(gtk_tree_model_get_column_type(cmodel, 1)
        == gtk_tree_model_get_column_type(model, 1),
        "should return existing column type (column 1)");
  check(gtk_tree_model_get_column_type(cmodel, 2) == G_TYPE_BOOLEAN,
        "should return boolean type for last column");
  check(kgtk3_combo_model_get_separator_column(cmodel_k) == 2,
        "should return index of separator column");

  /*
   * Existence and number of children
   */
  check(gtk_tree_model_iter_n_children(cmodel, NULL) == 3,
        "should return number of root children when NULL is passed to iter_n_children");

  gtk_tree_model_get_iter_first(cmodel, &top_level);
  check(!gtk_tree_model_iter_has_child(cmodel, &top_level),
        "should return no children if original has children - iter_has_child");
  check(gtk_tree_model_iter_n_children(cmodel, &top_level) == 0,
        "should return 0 children if original has children - iter_n_children");

  gtk_tree_model_iter_next(cmodel, &top_level);
  check(gtk_tree_model_iter_has_child(cmodel, &top_level),
        "should return two extra children if original has children - iter_has_child");
  check(gtk_tree_model_iter_n_children(cmodel, &top_level) == 5,
        "should return two extra children if original has children - iter_n_children");

  gtk_tree_model_iter_children(cmodel, &child, &top_level);
  check(!gtk_tree_model_iter_has_child(cmodel, &child),
        "should return no children for virtual items - header - iter_has_children");
  check(gtk_tree_model_iter_n_children(cmodel, &child) == 0,
        "should return no children for virtual items - header - iter_n_children");

  gtk_tree_model_iter_next(cmodel, &child);
  check(!gtk_tree_model_iter_has_child(cmodel, &child),
        "should return no children for virtual items - separator - iter_has_children");
  check(gtk_tree_model_iter_n_children(cmodel, &child) == 0,
        "should return no children for virtual items - separator - iter_n_children");

  /*
   * Getting child iters
   */
  gtk_tree_model_iter_children(cmodel, &top_level, NULL);
  check_col_str(cmodel, &top_level, "Root 1",
                "should return first iter for iter_children when passed NULL");
  gtk_tree_model_iter_nth_child(cmodel, &top_level, NULL, 2);
  check_col_str(cmodel, &top_level, "Root 3",
                "should return top level iter for iter_nth_child when passed NULL");

  gtk_tree_model_get_iter_first(cmodel, &top_level);
  check(!gtk_tree_model_iter_children(cmodel, &child, &top_level),
        "should return false for children when iter has no children - iter_children");
  check(!gtk_tree_model_iter_nth_child(cmodel, &child, &top_level, 3),
        "should return false for children when iter has no children - iter_nth_child");

  check(!gtk_tree_model_iter_nth_child(cmodel, &top_level, NULL, 4),
        "should return false for children when index is invalid - level 0");

  gtk_tree_model_iter_nth_child(cmodel, &top_level, NULL, 2);
  check(!gtk_tree_model_iter_nth_child(cmodel, &child, &top_level, 5),
        "should return false for children when index is invalid - level 1");

  gtk_tree_model_iter_nth_child(cmodel, &top_level, NULL, 1);
  gtk_tree_model_iter_children(cmodel, &child, &top_level);
  check(!gtk_tree_model_iter_children(cmodel, &subchild, &child),
        "should return false for children on virtual items");

  /*
   * Getting extra children with iter_nth_child
   */
  gtk_tree_model_iter_nth_child(cmodel, &top_level, NULL, 1);

  gtk_tree_model_iter_nth_child(cmodel, &child, &top_level, 0);
  check_col_int(cmodel, &child, 10,
                "should return extra children with nth child - parent header (col 0)");
  check_col_str(cmodel, &child, "Root 2",
                "should return extra children with nth child - parent header (col 1)");

  gtk_tree_model_iter_nth_child(cmodel, &child, &top_level, 2);
  check_col_int(cmodel, &child, 11,
                "should return extra children with nth child - existing item 1 (col 0)");
  check_col_str(cmodel, &child, "Child 2.1",
                "should return extra children with nth child - existing item 1 (col 1)");

  gtk_tree_model_iter_nth_child(cmodel, &child, &top_level, 3);
  check_col_int(cmodel, &child, 23,
                "should return extra children with nth child - existing item 2 (col 0)");
  check_col_str(cmodel, &child, "Child 2.2",
                "should return extra children with nth child - existing item 2 (col 1)");

  /*
   * Getting extra children with iter_next
   */
  gtk_tree_model_iter_nth_child(cmodel, &top_level, NULL, 1);

  gtk_tree_model_iter_children(cmodel, &child, &top_level);
  check_col_int(cmodel, &child, 10,
                "should return extra children with iter_next - parent header (col 0)");
  check_col_str(cmodel, &child, "Root 2",
                "should return extra children with iter_next - parent header (col 1)");

  gtk_tree_model_iter_next(cmodel, &child);
  /* Separator */

  gtk_tree_model_iter_next(cmodel, &child);
  check_col_int(cmodel, &child, 11,
                "should return extra children with iter_next - existing item 1 (col 0)");
  check_col_str(cmodel, &child, "Child 2.1",
                "should return extra children with iter_next - existing item 1 (col 1)");

  gtk_tree_model_iter_next(cmodel, &child);
  check_col_int(cmodel, &child, 23,
                "should return extra children with iter_next - existing item 2 (col 0)");
  check_col_str(cmodel, &child, "Child 2.2",
                "should return extra children with iter_next - existing item 2 (col 1)");

  gtk_tree_model_iter_next(cmodel, &child);
  check_col_int(cmodel, &child, 30,
                "should return extra children with iter_next - existing item 3 (col 0)");
  check_col_str(cmodel, &child, "Child 2.3",
                "should return extra children with iter_next - existing item 3 (col 1)");

  check(!gtk_tree_model_iter_next(cmodel, &child),
        "should return extra children with iter_next - no more children");


  /*
   * Extra separator column
   */
  gtk_tree_model_iter_nth_child(cmodel, &top_level, NULL, 1);
  check_col_bool(cmodel, &top_level, FALSE,
                 "should add extra separator column - level 0");

  gtk_tree_model_iter_children(cmodel, &child, &top_level);
  check_col_bool(cmodel, &child, FALSE,
                 "should add extra separator column - level 1, parent header");

  gtk_tree_model_iter_next(cmodel, &child);
  check_col_bool(cmodel, &child, TRUE,
                 "should add extra separator column - level 1, separator");

  gtk_tree_model_iter_next(cmodel, &child);
  check_col_bool(cmodel, &child, FALSE,
                 "should add extra separator column - level 1, existing item");


  /*
   * Deeper hierarchy
   */
  gtk_tree_model_iter_nth_child(cmodel, &top_level, NULL, 2);
  gtk_tree_model_iter_nth_child(cmodel, &child, &top_level, 2);

  gtk_tree_model_iter_children(cmodel, &subchild, &child);
  check_col_str(cmodel, &subchild, "Child 3.1",
                "should work with deeper hierarchies - level 2 header");

  gtk_tree_model_iter_next(cmodel, &subchild);
  check_col_bool(cmodel, &subchild, TRUE,
                 "should work with deeper hierarchies - level 2 separator");

  gtk_tree_model_iter_nth_child(cmodel, &subchild, &child, 2);
  check_col_str(cmodel, &subchild, "Child 3.1.1",
                "should work with deeper hierarchies - level 2 regular");

  check(!gtk_tree_model_iter_next(cmodel, &subchild),
        "should work with deeper hierarchies - no more children");

  check(!gtk_tree_model_iter_children(cmodel, &top_level, &subchild),
        "should work with deeper hierarchies - no level 3 children");


  /*
   * Parent
   */
  gtk_tree_model_get_iter_first(cmodel, &top_level);
  check(!gtk_tree_model_iter_parent(cmodel, &parent, &top_level),
        "should return no parent for root item");

  gtk_tree_model_iter_nth_child(cmodel, &top_level, NULL, 1);
  gtk_tree_model_iter_nth_child(cmodel, &child, &top_level, 3);
  gtk_tree_model_iter_parent(cmodel, &parent, &child);
  check_col_str(cmodel, &parent, "Root 2",
                "should return parent for regular item");

  gtk_tree_model_iter_nth_child(cmodel, &child, &top_level, 0);
  gtk_tree_model_iter_parent(cmodel, &parent, &child);
  check_col_str(cmodel, &parent, "Root 2",
                "should return parent for extra item");


  /*
   * Backwards iteration - root
   */
  gtk_tree_model_iter_nth_child(cmodel, &top_level, NULL, 2);
  check_col_str(cmodel, &top_level, "Root 3",
                "should iterate backwards in root - item 3");

  gtk_tree_model_iter_previous(cmodel, &top_level);
  check_col_str(cmodel, &top_level, "Root 2",
                "should iterate backwards in root - item 2");

  gtk_tree_model_iter_previous(cmodel, &top_level);
  check_col_str(cmodel, &top_level, "Root 1",
                "should iterate backwards in root - item 1");

  check(!gtk_tree_model_iter_previous(cmodel, &top_level),
        "should iterate backwards in root - no more items");


  /*
   * Backwards iteration - subtree
   */
  gtk_tree_model_iter_nth_child(cmodel, &top_level, NULL, 1);
  gtk_tree_model_iter_nth_child(cmodel, &child, &top_level, 3);
  check_col_str(cmodel, &child, "Child 2.2",
                "should iterate backwards in subtree - item 4");

  gtk_tree_model_iter_previous(cmodel, &child);
  check_col_str(cmodel, &child, "Child 2.1",
                "should iterate backwards in subtree - item 3");

  gtk_tree_model_iter_previous(cmodel, &child);
  check_col_bool(cmodel, &child, TRUE,
                "should iterate backwards in subtree - item 2 (separator)");

  gtk_tree_model_iter_previous(cmodel, &child);
  check_col_str(cmodel, &child, "Root 2",
                "should iterate backwards in subtree - item 1 (header)");

  check(!gtk_tree_model_iter_previous(cmodel, &child),
        "should iterate backwards in subtree - no more items");


  /*
   * Getting iter from path
   */
  path = gtk_tree_path_new_from_indices(2, -1);
  gtk_tree_model_get_iter(cmodel, &top_level, path);
  check_col_str(cmodel, &top_level, "Root 3",
                "should return iter from path - level 0");
  gtk_tree_path_free(path);

  path = gtk_tree_path_new_from_indices(1, 2, -1);
  gtk_tree_model_get_iter(cmodel, &child, path);
  check_col_str(cmodel, &child, "Child 2.1",
                "should return iter from path - level 1 regular");
  gtk_tree_path_free(path);

  path = gtk_tree_path_new_from_indices(2, 0, -1);
  gtk_tree_model_get_iter(cmodel, &child, path);
  check_col_str(cmodel, &child, "Root 3",
                "should return iter from path - level 1 virtual");
  gtk_tree_path_free(path);
  gtk_tree_model_iter_next(cmodel, &child);
  check_col_bool(cmodel, &child, TRUE,
        "should return iter from path - level 1 virtual - iter_next");

  path = gtk_tree_path_new_from_indices(2, 2, 2, -1);
  gtk_tree_model_get_iter(cmodel, &subchild, path);
  check_col_str(cmodel, &subchild, "Child 3.1.1",
                "should return iter from path - level 2 regular");
  gtk_tree_path_free(path);

  path = gtk_tree_path_new_from_indices(2, 2, 1, -1);
  gtk_tree_model_get_iter(cmodel, &subchild, path);
  check_col_bool(cmodel, &subchild, TRUE,
                "should return iter from path - level 2 virtual");
  gtk_tree_path_free(path);

  gtk_tree_model_iter_previous(cmodel, &subchild);
  check_col_str(cmodel, &subchild, "Child 3.1",
                "should return iter from path - level 2 virtual - iter_previous");

  path = gtk_tree_path_new_from_indices(4, -1);
  check(!gtk_tree_model_get_iter(cmodel, &top_level, path),
        "should return false for non existing path - level 0");
  gtk_tree_path_free(path);

  path = gtk_tree_path_new_from_indices(1, 10, -1);
  check(!gtk_tree_model_get_iter(cmodel, &top_level, path),
        "should return false for non existing path - level 1");
  gtk_tree_path_free(path);


  /*
   * Getting path from iter
   */
  gtk_tree_model_get_iter_first(cmodel, &top_level);
  check_path(cmodel, &top_level, "0",
             "should return path from iter - level 0");

  gtk_tree_model_iter_nth_child(cmodel, &top_level, NULL, 1);
  gtk_tree_model_iter_nth_child(cmodel, &child, &top_level, 2);
  check_path(cmodel, &child, "1:2",
             "should return path from iter - level 1 regular");

  gtk_tree_model_iter_previous(cmodel, &child);
  check_path(cmodel, &child, "1:1",
             "should return path from iter - level 1 separator");

  gtk_tree_model_iter_nth_child(cmodel, &top_level, NULL, 2);
  gtk_tree_model_iter_nth_child(cmodel, &child, &top_level, 2);
  gtk_tree_model_iter_nth_child(cmodel, &subchild, &child, 1);
  check_path(cmodel, &subchild, "2:2:1",
             "should return path from iter - level 2 separator");

  gtk_tree_model_iter_next(cmodel, &subchild);
  check_path(cmodel, &subchild, "2:2:2",
             "should return path from iter - level 2 regular");


  /*
   * End
   */
  return finalize();
}
