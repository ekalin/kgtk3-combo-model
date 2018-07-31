#include <gtk/gtk.h>


G_BEGIN_DECLS

#define K_TYPE_BUTTON (k_button_get_type())
G_DECLARE_FINAL_TYPE(KButton, k_button, K, BUTTON, GtkButton)

GtkWidget *k_button_new(gchar *label);

G_END_DECLS

struct _KButton
{
  GtkButton parent_instance;
};

G_DEFINE_TYPE(KButton, k_button, GTK_TYPE_BUTTON)

static void
k_button_init(KButton *kbutton)
{
}

static void
k_button_class_init(KButtonClass *class)
{
}

GtkWidget *
k_button_new(gchar *label)
{
  KButton *self = g_object_new(K_TYPE_BUTTON,
                               "always-show-image", TRUE,
                               NULL);
  gtk_button_set_label(GTK_BUTTON(self), label);

  GtkWidget *image = gtk_image_new_from_icon_name("document-new", GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image(GTK_BUTTON(self), image);

  return GTK_WIDGET(self);
}


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

  btn = k_button_new("Test!");
  g_signal_connect(btn, "clicked", G_CALLBACK(on_clicked), NULL);

  gtk_container_add(GTK_CONTAINER(window), btn);

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
