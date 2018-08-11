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

#include "kgtk3-combo-model.h"

/**
 * SECTION: kgtk3combomodel
 * @Title: KGtk3ComboModel
 * @Short_description: Wrapper for GtkTreeModel to be used in combos
 * when items with children need to be selected.
 *
 * Gtk+ 3.17.5 has removed the ability to select items in a
 * GtkComboBox that have children. In Gtk+ 2 an extra item was added
 * to the combos, allowing the parent item to be selected, but without
 * those items in Gtk+ 3 items with children cannot be selected with
 * the mouse.
 *
 * KGtk3ComboModel is a wrapper for GtkTreeModel that returns a view
 * of the wrapped model with extra items just like Gtk+ 2 used to do.
 *
 * To use it, wrap your GtkTreeModel in KGtk3ComboModel with:
 *
 * |[<!-- language="C" -->
 * GtkTreeModel *wrapped_model = GTK_TREE_MODEL(kgtk3_combo_model_new(model));
 * ]|
 *
 * and use `wrapped_model` in your combo. The wrapped model will
 * include extra itens corresponding to the parent item and a
 * separator.
 *
 * An extra boolean column is added after all the columns of the base
 * model. This column is `TRUE` for the separator items, and should be
 * used in a row separator function, something like this:
 *
 *
 * |[<!-- language="C" -->
 * gboolean is_separator(GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
 * {
 *   gboolean sep;
 *   gtk_tree_model_get(model, iter, LAST_COLUM+1, &sep, -1);
 *   return sep;
 * }
 * ...
 * gtk_combo_box_set_row_separator_func(GTK_COMBO_BOX(combo),
 *                                      is_separator, NULL, NULL);
 * ]|
 *
 *
 * ## Technical note
 *
 * KGtk3ComboModel uses `user_data3` in iterators to store its
 * internal data. If the base model uses this field, it cannot be
 * wrapped. None of the standard Gtk TreeModels use that field, so
 * it's safe to wrap ListStore, TreeStore, TreeModelFilter and
 * TreeModelSort.
 */

/*
 * Technical note:
 *
 * Iterators are essentially a copy of a base model's iterator, only
 * the user_data3 is used by KGtk3ComboModel as follows:
 *
 * - user_data3 == TYPE_REGULAR: Iter points to a regular item of the
 *   base model.
 *
 * - user_data3 == TYPE_HEADER: Iter points to the virtual "header"
 *   item that repeats the parent. The iter actually points to the
 *   parent item in the base model.
 *
 * - user_data3 == TYPE_HEADER: Similar to the above, but for the
 *   separator item.
 */

struct _KGtk3ComboModel
{
  GObject       parent_instance;
  GtkTreeModel *base_model;
  gint          separator_column;
};

#define TYPE_REGULAR   GINT_TO_POINTER(0)
#define TYPE_HEADER    GINT_TO_POINTER(1)
#define TYPE_SEPARATOR GINT_TO_POINTER(2)



static void kgtk3_combo_model_tree_model_init(GtkTreeModelIface *iface);
static void kgtk3_combo_model_dispose(GObject *object);

static GtkTreeModelFlags kgtk3_combo_model_get_flags(GtkTreeModel *model);
static gint kgtk3_combo_model_get_n_columns(GtkTreeModel *model);
static GType kgtk3_combo_model_get_column_type(GtkTreeModel *model, gint index);
static gboolean kgtk3_combo_model_get_iter(GtkTreeModel *model, GtkTreeIter *iter, GtkTreePath *path);
static GtkTreePath *kgtk3_combo_model_get_path(GtkTreeModel *model, GtkTreeIter *iter);
static void kgtk3_combo_model_get_value(GtkTreeModel *model, GtkTreeIter *iter, gint column, GValue *value);
static gboolean kgtk3_combo_model_iter_next(GtkTreeModel *model, GtkTreeIter *iter);
static gboolean kgtk3_combo_model_iter_children(GtkTreeModel *model, GtkTreeIter *iter, GtkTreeIter *parent);
static gboolean kgtk3_combo_model_iter_has_child(GtkTreeModel *model, GtkTreeIter *iter);
static gint kgtk3_combo_model_iter_n_children(GtkTreeModel *model, GtkTreeIter *iter);
static gboolean kgtk3_combo_model_iter_nth_child(GtkTreeModel *model, GtkTreeIter *iter, GtkTreeIter *parent, gint n);
static gboolean kgtk3_combo_model_iter_parent(GtkTreeModel *model, GtkTreeIter *iter, GtkTreeIter *child);


G_DEFINE_TYPE_WITH_CODE(KGtk3ComboModel, kgtk3_combo_model, G_TYPE_OBJECT,
                        G_IMPLEMENT_INTERFACE(GTK_TYPE_TREE_MODEL,
                                              kgtk3_combo_model_tree_model_init))


static
void
kgtk3_combo_model_init(KGtk3ComboModel *model)
{
}


static
void
kgtk3_combo_model_class_init(KGtk3ComboModelClass *klass)
{
  GObjectClass *object_klass = G_OBJECT_CLASS(klass);
  object_klass->dispose = kgtk3_combo_model_dispose;
}


static
void
kgtk3_combo_model_tree_model_init(GtkTreeModelIface *iface)
{
  iface->get_flags = kgtk3_combo_model_get_flags;
  iface->get_n_columns = kgtk3_combo_model_get_n_columns;
  iface->get_column_type = kgtk3_combo_model_get_column_type;
  iface->get_iter = kgtk3_combo_model_get_iter;
  iface->get_path = kgtk3_combo_model_get_path;
  iface->get_value = kgtk3_combo_model_get_value;
  iface->iter_next = kgtk3_combo_model_iter_next;
  /* Not necessary - default implementation works
  iface->iter_previous = gtk_tree_model_filter_iter_previous;
  */
  iface->iter_children = kgtk3_combo_model_iter_children;
  iface->iter_has_child = kgtk3_combo_model_iter_has_child;
  iface->iter_n_children = kgtk3_combo_model_iter_n_children;
  iface->iter_nth_child = kgtk3_combo_model_iter_nth_child;
  iface->iter_parent = kgtk3_combo_model_iter_parent;
  /* Not necessary
  iface->ref_node = gtk_tree_model_filter_ref_node;
  iface->unref_node = gtk_tree_model_filter_unref_node;
  */
}


/**
 * kgtk3_combo_model_new:
 * @base_model: Base model
 *
 * Creates a new #KGtk3ComboModel-struct.
 *
 * A reference to the base model will be held by the #KGtk3ComboModel-struct.
 *
 * Returns: A newly-created #KGtk3ComboModel-struct.
 */
KGtk3ComboModel *
kgtk3_combo_model_new(GtkTreeModel *base_model)
{
  g_return_val_if_fail(GTK_IS_TREE_MODEL(base_model), NULL);

  KGtk3ComboModel *self = g_object_new(KGTK3_TYPE_COMBO_MODEL, NULL);
  self->base_model = base_model;
  self->separator_column = gtk_tree_model_get_n_columns(base_model);

  g_object_ref(base_model);

  return self;
}


static
void
kgtk3_combo_model_dispose(GObject *object)
{
  g_object_unref(KGTK3_COMBO_MODEL(object)->base_model);
  G_OBJECT_CLASS(kgtk3_combo_model_parent_class)->dispose(object);
}


/**
 * kgtk3_combo_model_get_separator_column:
 * @model: A #KGtk3ComboModel-struct
 *
 * Returns: The index of the column indicating whether a row is a
 * separator.
 */
gint
kgtk3_combo_model_get_separator_column(KGtk3ComboModel *model)
{
  return model->separator_column;
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
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), G_TYPE_INVALID);
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
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), FALSE);

  gint depth = gtk_tree_path_get_depth(path);
  if (depth == 1) {
    gboolean ret = gtk_tree_model_get_iter(KGTK3_COMBO_MODEL(model)->base_model, iter, path);
    iter->user_data3 = TYPE_REGULAR;
    return ret;
  }

  gint *indices = gtk_tree_path_get_indices(path);

  GtkTreePath *new_path = gtk_tree_path_new();
  gtk_tree_path_append_index(new_path, indices[0]);
  for (int i = 1; i < depth - 1; ++i) {
    gtk_tree_path_append_index(new_path, indices[i] - 2);
  }
  gint last_index = -1;
  if (indices[depth - 1] >= 2) {
    gtk_tree_path_append_index(new_path, indices[depth - 1] - 2);
  } else {
    last_index = indices[depth - 1];
  }

  gboolean ret = gtk_tree_model_get_iter(KGTK3_COMBO_MODEL(model)->base_model, iter, new_path);
  gtk_tree_path_free(new_path);
  if (!ret) {
    return FALSE;
  }

  if (last_index == 0) {
    iter->user_data3 = TYPE_HEADER;
  } else if (last_index == 1) {
    iter->user_data3 = TYPE_SEPARATOR;
  } else {
    iter->user_data3 = TYPE_REGULAR;
  }

  return TRUE;
}


static
GtkTreePath *
kgtk3_combo_model_get_path(GtkTreeModel *model, GtkTreeIter *iter)
{
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), NULL);

  GtkTreePath *base_path = gtk_tree_model_get_path(KGTK3_COMBO_MODEL(model)->base_model, iter);
  if (base_path == NULL) {
    return NULL;
  }

  gint depth;
  gint *indices = gtk_tree_path_get_indices_with_depth(base_path, &depth);

  GtkTreePath *path = gtk_tree_path_new();
  gtk_tree_path_append_index(path, indices[0]);
  for (int i = 1; i < depth; ++i) {
    gtk_tree_path_append_index(path, indices[i] + 2);
  }
  gtk_tree_path_free(base_path);

  if (iter->user_data3 == TYPE_HEADER) {
    gtk_tree_path_append_index(path, 0);
  } else if (iter->user_data3 == TYPE_SEPARATOR) {
    gtk_tree_path_append_index(path, 1);
  }

  return path;
}


static
void
kgtk3_combo_model_get_value(GtkTreeModel *model, GtkTreeIter *iter, gint column, GValue *value)
{
  g_return_if_fail(KGTK3_IS_COMBO_MODEL(model));
  KGtk3ComboModel *cmodel = KGTK3_COMBO_MODEL(model);

  if (column == cmodel->separator_column) {
    g_value_init(value, G_TYPE_BOOLEAN);
    g_value_set_boolean(value, iter->user_data3 == TYPE_SEPARATOR);
  } else {
    return gtk_tree_model_get_value(cmodel->base_model, iter, column, value);
  }
}


static
gboolean
kgtk3_combo_model_iter_next(GtkTreeModel *model, GtkTreeIter *iter)
{
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), FALSE);
  KGtk3ComboModel *cmodel = KGTK3_COMBO_MODEL(model);

  if (iter->user_data3 == TYPE_HEADER) {
    iter->user_data3 = TYPE_SEPARATOR;
    return TRUE;
  } else if (iter->user_data3 == TYPE_SEPARATOR) {
    GtkTreeIter child;
    gtk_tree_model_iter_children(cmodel->base_model, &child, iter);
    *iter = child;
    iter->user_data3 = TYPE_REGULAR;
    return TRUE;
  } else {
    gboolean ret = gtk_tree_model_iter_next(cmodel->base_model, iter);
    iter->user_data3 = TYPE_REGULAR;
    return ret;
  }
}


static
gboolean
kgtk3_combo_model_iter_children(GtkTreeModel *model, GtkTreeIter *iter, GtkTreeIter *parent)
{
  return kgtk3_combo_model_iter_nth_child(model, iter, parent, FALSE);
}


static
gboolean
kgtk3_combo_model_iter_has_child(GtkTreeModel *model, GtkTreeIter *iter)
{
  return kgtk3_combo_model_iter_n_children(model, iter) > 0;
}


static
gint
kgtk3_combo_model_iter_n_children(GtkTreeModel *model, GtkTreeIter *iter)
{
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), 0);

  if (iter && (iter->user_data3 == TYPE_HEADER || iter->user_data3 == TYPE_SEPARATOR)) {
    return 0;
  }

  gint n_children = gtk_tree_model_iter_n_children(KGTK3_COMBO_MODEL(model)->base_model, iter);
  if (iter == NULL) {
    return n_children;
  } else {
    return n_children > 0 ? n_children + 2 : 0;
  }
}


static
gboolean
kgtk3_combo_model_iter_nth_child(GtkTreeModel *model, GtkTreeIter *iter, GtkTreeIter *parent, gint n)
{
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), FALSE);
  KGtk3ComboModel *cmodel = KGTK3_COMBO_MODEL(model);

  if (parent == NULL) {
    gboolean ret = gtk_tree_model_iter_nth_child(cmodel->base_model, iter, NULL, n);
    iter->user_data3 = TYPE_REGULAR;
    return ret;
  } else {
    if (!kgtk3_combo_model_iter_has_child(GTK_TREE_MODEL(cmodel), parent)) {
      return FALSE;
    }

    if (n == 0 || n == 1) {
      *iter = *parent;
      iter->user_data3 = n == 0 ? TYPE_HEADER : TYPE_SEPARATOR;
      return TRUE;
    } else {
      gboolean ret = gtk_tree_model_iter_nth_child(cmodel->base_model, iter, parent, n - 2);
      iter->user_data3 = TYPE_REGULAR;
      return ret;
    }
  }
}


static
gboolean
kgtk3_combo_model_iter_parent(GtkTreeModel *model, GtkTreeIter *iter, GtkTreeIter *child)
{
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), FALSE);

  if (child->user_data3 == TYPE_HEADER || child->user_data3 == TYPE_SEPARATOR) {
    *iter = *child;
    iter->user_data3 = TYPE_REGULAR;
    return TRUE;
  } else {
    gboolean ret = gtk_tree_model_iter_parent(KGTK3_COMBO_MODEL(model)->base_model, iter, child);
    iter->user_data3 = TYPE_REGULAR;
    return ret;
  }
}
