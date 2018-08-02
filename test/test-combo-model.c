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
  gtk_init(&argc, &argv);

  /*
   * Construction
   */
  check(kgtk3_combo_model_new(NULL) == NULL,
        "should fail when the constructor receives NULL");

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  check(kgtk3_combo_model_new((GtkTreeModel *) window) == NULL,
        "should fail when the constructor receives an invalid object");


  if (errors) {
    printf("%d checks FAILED\n", errors);
  } else {
    printf("all checks passed\n");
  }
  return errors;
}
