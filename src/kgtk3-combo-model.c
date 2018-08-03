#include <gtk/gtk.h>

#include "kgtk3-combo-model.h"


struct _KGtk3ComboModel
{
  GObject       parent_instance;
  GtkTreeModel *base_model;
  gint          separator_column;
};


static void kgtk3_combo_model_tree_model_init(GtkTreeModelIface *iface);
static GtkTreeModelFlags kgtk3_combo_model_get_flags(GtkTreeModel *model);
static gint kgtk3_combo_model_get_n_columns(GtkTreeModel *model);
static GType kgtk3_combo_model_get_column_type(GtkTreeModel *model, gint index);
static gboolean kgtk3_combo_model_get_iter(GtkTreeModel *model, GtkTreeIter *iter, GtkTreePath *path);
static gboolean kgtk3_combo_model_iter_next(GtkTreeModel *model, GtkTreeIter *iter);
static gint kgtk3_combo_model_iter_n_children(GtkTreeModel *model, GtkTreeIter *iter);


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
  iface->get_n_columns = kgtk3_combo_model_get_n_columns;
  iface->get_column_type = kgtk3_combo_model_get_column_type;
  iface->get_iter = kgtk3_combo_model_get_iter;
  /*
  iface->get_path = gtk_tree_model_filter_get_path;
  iface->get_value = gtk_tree_model_filter_get_value;
  */
  iface->iter_next = kgtk3_combo_model_iter_next;
  /*
  iface->iter_previous = gtk_tree_model_filter_iter_previous;
  iface->iter_children = gtk_tree_model_filter_iter_children;
  iface->iter_has_child = gtk_tree_model_filter_iter_has_child;
  */
  iface->iter_n_children = kgtk3_combo_model_iter_n_children;
  /*
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
  self->separator_column = gtk_tree_model_get_n_columns(base_model);

  return self;
}


static
GtkTreeModelFlags
kgtk3_combo_model_get_flags(GtkTreeModel *model)
{
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), 0);

  return gtk_tree_model_get_flags(KGTK3_COMBO_MODEL(model)->base_model);
}


static
gint
kgtk3_combo_model_get_n_columns(GtkTreeModel *model)
{
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), 0);

  return gtk_tree_model_get_n_columns(KGTK3_COMBO_MODEL(model)->base_model) + 1;
}


static
GType
kgtk3_combo_model_get_column_type(GtkTreeModel *model, gint index)
{
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), 0);
  KGtk3ComboModel *cmodel = KGTK3_COMBO_MODEL(model);

  if (index == cmodel->separator_column) {
    return G_TYPE_BOOLEAN;
  } else {
    return gtk_tree_model_get_column_type(cmodel->base_model, index);
  }
}


static
gboolean
kgtk3_combo_model_get_iter(GtkTreeModel *model, GtkTreeIter *iter, GtkTreePath *path)
{
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), 0);

  // TODO
  return gtk_tree_model_get_iter(KGTK3_COMBO_MODEL(model)->base_model, iter, path);
}


static
gboolean
kgtk3_combo_model_iter_next(GtkTreeModel *model, GtkTreeIter *iter)
{
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), 0);

  // TODO
  return gtk_tree_model_iter_next(KGTK3_COMBO_MODEL(model)->base_model, iter);
}


static
gint
kgtk3_combo_model_iter_n_children(GtkTreeModel *model, GtkTreeIter *iter)
{
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), 0);

  gint n_children = gtk_tree_model_iter_n_children(KGTK3_COMBO_MODEL(model)->base_model, iter);
  // TODO

  if (iter == NULL) {
    return n_children;
  } else {
    return n_children > 0 ? n_children + 2 : 0;
  }
}
