#ifndef __KGTK3_COMBO_MODEL_H__
#define __KGTK3_COMBO_MODEL_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KGTK3_TYPE_COMBO_MODEL (kgtk3_combo_model_get_type())
G_DECLARE_FINAL_TYPE(KGtk3ComboModel, kgtk3_combo_model, KGTK3, COMBO_MODEL, GObject)

KGtk3ComboModel *kgtk3_combo_model_new(GtkTreeModel *base_model);

G_END_DECLS

#endif /* __KGTK3_COMBO_MODEL_H__ */
