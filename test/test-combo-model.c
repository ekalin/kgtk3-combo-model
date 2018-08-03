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


int
main(int argc, char *argv[])
{
  /*
   * Set up
   */
  GtkTreeIter top_level, child, subchild;

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
