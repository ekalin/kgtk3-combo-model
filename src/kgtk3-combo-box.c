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
#include "kgtk3-combo-box.h"

/**
 * SECTION: kgtk3combomodel
 * @Title: KGtk3ComboBox
 * @Short_description: TODO
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

struct _KGtk3ComboBox
{
  GtkComboBox   parent_instance;
  GtkTreeModel *base_model;
};


static gboolean is_separator(GtkTreeModel *model, GtkTreeIter *iter, gpointer data);


G_DEFINE_TYPE(KGtk3ComboBox, kgtk3_combo_box, GTK_TYPE_COMBO_BOX)


static
void
kgtk3_combo_box_init(KGtk3ComboBox *combo_box)
{
}


static
void
kgtk3_combo_box_class_init(KGtk3ComboBoxClass *klass)
{
}


/**
 * kgtk3_combo_box_new:
 *
 * Creates a new #KGtk3ComboBox-struct.
 *
 * Returns: A newly-created #KGtk3ComboBox-struct.
 */
GtkWidget *
kgtk3_combo_box_new()
{
  return g_object_new(KGTK3_TYPE_COMBO_BOX, NULL);
}


/**
 * kgtk3_combo_box_new_with_model:
 * @model: A GtkTreeModel
 *
 * Creates a new #KGtk3ComboBox-struct.
 *
 * The model will be wrapped in a #KGtk3ComboModel-struct.
 *
 * Returns: A newly-created #KGtk3ComboBox-struct.
 */
GtkWidget *
kgtk3_combo_box_new_with_model(GtkTreeModel *model)
{
  g_return_val_if_fail(GTK_IS_TREE_MODEL(model), NULL);

  GtkTreeModel *cmodel = GTK_TREE_MODEL(kgtk3_combo_model_new(model));

  KGtk3ComboBox *self = g_object_new(KGTK3_TYPE_COMBO_BOX, "model", cmodel, NULL);

  gtk_combo_box_set_row_separator_func(GTK_COMBO_BOX(self),
                                       is_separator, NULL, NULL);

  return GTK_WIDGET(self);
}


/**
 * kgtk3_combo_box_set_model:
 * @combo: A #KGtk3ComboBox-struct
 * @model: A GtkTreeModel
 *
 * Sets the model used by @combo_box to be @model. Will unset a
 * previously set model (if applicable). If model is %NULL, then it
 * will unset the model.
 *
 * Note that this function does not clear the cell renderers, you have
 * to call gtk_cell_layout_clear() yourself if you need to set up
 * different cell renderers for the new model.
 *
 * The model will be wrapped in a #KGtk3ComboModel-struct.
 */
void
kgtk3_combo_box_set_model(KGtk3ComboBox *combo, GtkTreeModel *model)
{
  g_return_if_fail(KGTK3_IS_COMBO_BOX(combo));
  g_return_if_fail(GTK_IS_TREE_MODEL(model));

  if (model == combo->base_model) {
    return;
  }

  combo->base_model = model;
  GtkTreeModel *cmodel = GTK_TREE_MODEL(kgtk3_combo_model_new(model));
  gtk_combo_box_set_model(GTK_COMBO_BOX(combo), cmodel);
}



static gboolean is_separator(GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
  g_return_val_if_fail(KGTK3_IS_COMBO_MODEL(model), FALSE);
  KGtk3ComboModel *cmodel = KGTK3_COMBO_MODEL(model);
  gint separator_column = kgtk3_combo_model_get_separator_column(cmodel);

  gboolean is_separator;
  gtk_tree_model_get(model, iter, separator_column, &is_separator, -1);
  return is_separator;
}
