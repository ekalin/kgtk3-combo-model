#ifndef _k_button_h_
#define _k_button_h_

G_BEGIN_DECLS

#define K_TYPE_BUTTON (k_button_get_type())
G_DECLARE_FINAL_TYPE(KButton, k_button, K, BUTTON, GtkButton)

GtkWidget *k_button_new(gchar *label);

G_END_DECLS

#endif /* _k_button_h_ */
