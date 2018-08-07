#include <gtk/gtk.h>

#include <kgtk3-combo-model.h>


GtkTreeModel *create_model()
{
  GtkTreeStore *model;
  GtkTreeIter iter, child;

  model = gtk_tree_store_new(1, G_TYPE_STRING);

  gtk_tree_store_append(model, &iter, NULL);
  gtk_tree_store_set(model, &iter, 0, "Item with no children", -1);

  gtk_tree_store_append(model, &iter, NULL);
  gtk_tree_store_set(model, &iter, 0, "Item with children", -1);

  gtk_tree_store_append(model, &child, &iter);
  gtk_tree_store_set(model, &child, 0, "Child 1", -1);

  gtk_tree_store_append(model, &child, &iter);
  gtk_tree_store_set(model, &child, 0, "Child 2", -1);

  KGtk3ComboModel *cmodel = kgtk3_combo_model_new(GTK_TREE_MODEL(model));
  g_object_unref(model);
  return GTK_TREE_MODEL(cmodel);
}


void on_changed(GtkComboBox *widget, gpointer data)
{
  GtkComboBox *combo = widget;
  GtkTreeIter iter;
  gchar *selected_text;

  if (gtk_combo_box_get_active_iter(combo, &iter)) {
    gtk_tree_model_get(gtk_combo_box_get_model(combo), &iter,
                       0, &selected_text, -1);
    printf("Selected: %s\n", selected_text);
  }
}


gboolean is_separator(GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
  gboolean sep;
  gtk_tree_model_get(model, iter, 1, &sep, -1);
  return sep;
}


GtkWidget *create_combo(GtkTreeModel *model)
{
  GtkWidget *combo;
  GtkCellRenderer *column;

  combo = gtk_combo_box_new_with_model(model);
  g_object_unref(model);

  gtk_combo_box_set_row_separator_func(GTK_COMBO_BOX(combo),
                                       is_separator, NULL, NULL);

  column = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), column, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), column,
                                 "text", 0, NULL);

  g_signal_connect(combo, "changed", G_CALLBACK(on_changed), NULL);

  return combo;
}


int main(int argc, char **argv)
{
  GtkWidget *window, *combo;
  GtkTreeModel *model;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "delete_event", G_CALLBACK(gtk_main_quit), NULL);

  model = create_model();
  combo = create_combo(model);

  gtk_container_add(GTK_CONTAINER(window), combo);

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
