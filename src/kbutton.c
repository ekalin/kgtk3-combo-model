#include <gtk/gtk.h>

#include "kbutton.h"


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
