#include <gtk/gtk.h>

#include "kbutton.h"


void on_clicked(GtkButton *button, gpointer data)
{
  printf("Clicked!\n");
}


int main(int argc, char **argv)
{
  GtkWidget *window, *btn;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "delete_event", G_CALLBACK(gtk_main_quit), NULL);

  btn = kgtk3_combo_model_button_new("Test!");
  g_signal_connect(btn, "clicked", G_CALLBACK(on_clicked), NULL);

  gtk_container_add(GTK_CONTAINER(window), btn);

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
