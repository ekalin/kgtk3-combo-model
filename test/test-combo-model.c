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
                     0, 1, 1, "Root 1", -1);

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
  check(gtk_tree_model_get_flags(model) == gtk_tree_model_get_flags(cmodel),
        "should return the same flags");


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
