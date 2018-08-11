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
#include <gtk/gtk.h>

#include <kgtk3-combo-box.h>


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

  return GTK_TREE_MODEL(model);
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


GtkWidget *create_combo(GtkTreeModel *model)
{
  GtkWidget *combo;
  GtkCellRenderer *column;

  combo = kgtk3_combo_box_new_with_model(model);
  g_object_unref(model);

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
