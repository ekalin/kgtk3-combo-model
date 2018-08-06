#include <stdio.h>

#include <kgtk3-combo-model.h>


int errors = 0;
void check(int result, const char *msg)
{
  printf("%s: ", msg);
  if (result) {
    printf("ok\n");
  } else {
    printf("FAILED\n");
    ++errors;
  }
}

void check_col_int(GtkTreeModel *model, GtkTreeIter *iter, gint expected, const char *msg)
{
  gint value;
  gtk_tree_model_get(model, iter, 0, &value, -1);
  check(expected == value, msg);
}

void check_col_bool(GtkTreeModel *model, GtkTreeIter *iter, gboolean expected, const char *msg)
{
  gboolean value;
  gtk_tree_model_get(model, iter, 2, &value, -1);
  check(expected == value, msg);
}

void check_col_str(GtkTreeModel *model, GtkTreeIter *iter, const char *expected, const char *msg)
{
  gchar *value;
  gtk_tree_model_get(model, iter, 1, &value, -1);
  check(strcmp(expected, value) == 0, msg);
  g_free(value);
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
  GtkTreeStore *model_store = gtk_tree_store_new(2,
                                                 G_TYPE_INT,
                                                 G_TYPE_STRING);

  gtk_tree_store_append(model_store, &top_level, NULL);
  gtk_tree_store_set(model_store, &top_level,
                     0, 1,
                     1, "Root 1", -1);

  gtk_tree_store_append(model_store, &top_level, NULL);
  gtk_tree_store_set(model_store, &top_level,
                     0, 10,
                     1, "Root 2", -1);
  gtk_tree_store_append(model_store, &child, &top_level);
  gtk_tree_store_set(model_store, &child,
                     0, 11,
                     1, "Child 2.1", -1);
  gtk_tree_store_append(model_store, &child, &top_level);
  gtk_tree_store_set(model_store, &child,
                     0, 23,
                     1, "Child 2.2", -1);
  gtk_tree_store_append(model_store, &child, &top_level);
  gtk_tree_store_set(model_store, &child,
                     0, 30,
                     1, "Child 2.3", -1);

  gtk_tree_store_append(model_store, &top_level, NULL);
  gtk_tree_store_set(model_store, &top_level,
                     0, 5,
                     1, "Root 3", -1);
  gtk_tree_store_append(model_store, &child, &top_level);
  gtk_tree_store_set(model_store, &child,
                     0, 6,
                     1, "Child 3.1", -1);
  gtk_tree_store_append(model_store, &subchild, &child);
  gtk_tree_store_set(model_store, &subchild,
                     0, 16,
                     1, "Child 3.1.1", -1);
  gtk_tree_store_append(model_store, &child, &top_level);
  gtk_tree_store_set(model_store, &child,
                     0, 7,
                     1, "Child 3.2", -1);

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
   * Getting iter from paths
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

  path = gtk_tree_path_new_from_indices(4, -1);
  check(!gtk_tree_model_get_iter(cmodel, &top_level, path),
        "should return false for non existing path - level 0");
  gtk_tree_path_free(path);

  path = gtk_tree_path_new_from_indices(1, 10, -1);
  check(!gtk_tree_model_get_iter(cmodel, &top_level, path),
        "should return false for non existing path - level 1");
  gtk_tree_path_free(path);


  /*
   * End
   */
  if (errors) {
    printf("%d checks FAILED\n", errors);
  } else {
    printf("all checks passed\n");
  }
  return errors;
}
