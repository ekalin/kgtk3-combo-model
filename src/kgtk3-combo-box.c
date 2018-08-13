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
 * @Short_description: Combo box allowing items with children to be selected.
 *
 * Gtk+ 3.17.5 has removed the ability to select items in a
 * GtkComboBox that have children. In Gtk+ 2 an extra item was added
 * to the combos, allowing the parent item to be selected, but without
 * those items in Gtk+ 3 items with children cannot be selected with
 * the mouse.
 *
 * KGtk3ComboBox is a subclass of GtkComboBox that wraps its model in
 * a #KGtk3ComboModel-struct and sets up a row separator function
 * automatically.
 *
 * To use it, just replace your GtkComboBox with KGtk3ComboBox. Since
 * it's a subclass, all methods of GtkComboBox still work.
 *
 * Note that the model used by the combo is not the model supplied in
 * the constructor or with kgtk3_combo_box_set_model(), but a
 * #KGtk3ComboModel-struct. For basic operations such as getting data
 * from the current iter, using the original model should work. But if
 * you want to do other operations (such as moving the iter around),
 * you should get the wrapped model (use the standard
 * gtk_combo_box_get_model() function) and use that instead of the
 * original model.
 *
 * ## Technical note
 *
 * #KGtk3ComboModel-struct (used internally by KGtk3ComboBox) uses
 * `user_data3` in iterators to store its internal data. If the base
 * model uses this field, it cannot be wrapped. None of the standard
 * Gtk TreeModels use that field, so it's safe to wrap ListStore,
 * TreeStore, TreeModelFilter and TreeModelSort.
 */

struct _KGtk3ComboBox
{
  GtkComboBox   parent_instance;
  GtkTreeModel *base_model;
};

enum {
  PROP_0,
  PROP_MODEL,
};


static void kgtk3_combo_box_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void kgtk3_combo_box_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);

static gboolean is_separator(GtkTreeModel *model, GtkTreeIter *iter, gpointer data);


G_DEFINE_TYPE(KGtk3ComboBox, kgtk3_combo_box, GTK_TYPE_COMBO_BOX)


static
void
kgtk3_combo_box_init(KGtk3ComboBox *combo_box)
{
  gtk_combo_box_set_row_separator_func(GTK_COMBO_BOX(combo_box),
                                       is_separator, NULL, NULL);
}


static
void
kgtk3_combo_box_class_init(KGtk3ComboBoxClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->set_property = kgtk3_combo_box_set_property;
  object_class->get_property = kgtk3_combo_box_get_property;

  /**
   * GtkComboBox:model:
   *
   * The model from which the combo box takes the values shown
   * in the list.
   *
   * When setting a model, it is wrapped in a #KGtk3ComboModel-struct,
   * but when getting the model, the wrapped model is returned.
   */
  g_object_class_install_property(
    object_class,
    PROP_MODEL,
    g_param_spec_object("model",
                        "ComboBox model",
                        "The model for the combo box",
                        GTK_TYPE_TREE_MODEL,
                        G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY));
}


static
void
kgtk3_combo_box_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
  KGtk3ComboBox *combo = KGTK3_COMBO_BOX(object);

  switch (prop_id) {
  case PROP_MODEL:
    kgtk3_combo_box_set_model(combo, g_value_get_object(value));
    break;

  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}


static
void
kgtk3_combo_box_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
  KGtk3ComboBox *combo = KGTK3_COMBO_BOX(object);

  switch (prop_id) {
  case PROP_MODEL:
    g_value_set_object(value, gtk_combo_box_get_model(GTK_COMBO_BOX(combo)));
    break;

  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
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

  return GTK_WIDGET(g_object_new(KGTK3_TYPE_COMBO_BOX, "model", model, NULL));
}


/**
 * kgtk3_combo_box_new_with_entry:
 *
 * Creates a new #KGtk3ComboBox-struct with an entry.
 *
 * Returns: A newly-created #KGtk3ComboBox-struct.
 */
GtkWidget *
kgtk3_combo_box_new_with_entry()
{
  return GTK_WIDGET(g_object_new(KGTK3_TYPE_COMBO_BOX, "has-entry", TRUE, NULL));
}



/**
 * kgtk3_combo_box_new_with_model_and_entry:
 * @model: A GtkTreeModel
 *
 * Creates a new #KGtk3ComboBox-struct with an entry.
 *
 * The model will be wrapped in a #KGtk3ComboModel-struct.
 *
 * Returns: A newly-created #KGtk3ComboBox-struct.
 */
GtkWidget *
kgtk3_combo_box_new_with_model_and_entry(GtkTreeModel *model)
{
  g_return_val_if_fail(GTK_IS_TREE_MODEL(model), NULL);

  return GTK_WIDGET(g_object_new(KGTK3_TYPE_COMBO_BOX,
                                 "has-entry", TRUE,
                                 "model", model,
                                 NULL));
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

  g_object_notify(G_OBJECT(combo), "model");
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
