#ifndef _k_button_h_
#define _k_button_h_

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KGTK3_TYPE_COMBO_MODEL_BUTTON (kgtk3_combo_model_button_get_type())
G_DECLARE_FINAL_TYPE(KGtk3ComboModelButton, kgtk3_combo_model_button, KGTK3, COMBO_MODEL_BUTTON, GtkButton)

GtkWidget *kgtk3_combo_model_button_new(gchar *label);

G_END_DECLS

#endif /* _k_button_h_ */
