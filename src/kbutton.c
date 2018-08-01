#include <gtk/gtk.h>

#include "kbutton.h"


struct _KGtk3ComboModelButton
{
  GtkButton parent_instance;
};

G_DEFINE_TYPE(KGtk3ComboModelButton, kgtk3_combo_model_button, GTK_TYPE_BUTTON)


static void
kgtk3_combo_model_button_init(KGtk3ComboModelButton *kbutton)
{
}


static void
kgtk3_combo_model_button_class_init(KGtk3ComboModelButtonClass *class)
{
}


GtkWidget *
kgtk3_combo_model_button_new(gchar *label)
{
  KGtk3ComboModelButton *self = g_object_new(KGTK3_TYPE_COMBO_MODEL_BUTTON,
                                             "always-show-image", TRUE,
                                             NULL);
  gtk_button_set_label(GTK_BUTTON(self), label);

  GtkWidget *image = gtk_image_new_from_icon_name("document-new", GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image(GTK_BUTTON(self), image);

  return GTK_WIDGET(self);
}
