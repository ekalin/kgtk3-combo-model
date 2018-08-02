#include <gtk/gtk.h>

#include "kgtk3-combo-model.h"


struct _KGtk3ComboModel
{
  GObject       parent_instance;
  GtkTreeModel *base_model;
};


static void kgtk3_combo_model_tree_model_init(GtkTreeModelIface *iface);
static GtkTreeModelFlags kgtk3_combo_model_get_flags(GtkTreeModel *model);


G_DEFINE_TYPE_WITH_CODE(KGtk3ComboModel, kgtk3_combo_model, G_TYPE_OBJECT,
                        G_IMPLEMENT_INTERFACE(GTK_TYPE_TREE_MODEL,
                                              kgtk3_combo_model_tree_model_init))


static
void
kgtk3_combo_model_init(KGtk3ComboModel *kbutton)
{
}


static
void
kgtk3_combo_model_class_init(KGtk3ComboModelClass *klass)
{
}


static
void
kgtk3_combo_model_tree_model_init(GtkTreeModelIface *iface)
{
  iface->get_flags = kgtk3_combo_model_get_flags;
  /*
  iface->get_n_columns = gtk_tree_model_filter_get_n_columns;
  iface->get_column_type = gtk_tree_model_filter_get_column_type;
  iface->get_iter = gtk_tree_model_filter_get_iter;
  iface->get_path = gtk_tree_model_filter_get_path;
  iface->get_value = gtk_tree_model_filter_get_value;
  iface->iter_next = gtk_tree_model_filter_iter_next;
  iface->iter_previous = gtk_tree_model_filter_iter_previous;
  iface->iter_children = gtk_tree_model_filter_iter_children;
  iface->iter_has_child = gtk_tree_model_filter_iter_has_child;
  iface->iter_n_children = gtk_tree_model_filter_iter_n_children;
  iface->iter_nth_child = gtk_tree_model_filter_iter_nth_child;
  iface->iter_parent = gtk_tree_model_filter_iter_parent;
  iface->ref_node = gtk_tree_model_filter_ref_node;
  iface->unref_node = gtk_tree_model_filter_unref_node;
  */
}


KGtk3ComboModel *
kgtk3_combo_model_new(GtkTreeModel *base_model)
{
  g_return_val_if_fail(GTK_IS_TREE_MODEL(base_model), NULL);

  KGtk3ComboModel *self = g_object_new(KGTK3_TYPE_COMBO_MODEL, NULL);
  self->base_model = base_model;

  return self;
}


static
GtkTreeModelFlags
kgtk3_combo_model_get_flags(GtkTreeModel *model)
{
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), 0);

  return gtk_tree_model_get_flags(KGTK3_COMBO_MODEL(model)->base_model);
}
