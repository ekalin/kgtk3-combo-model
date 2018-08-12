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
#include <kgtk3-combo-box.h>

#include "test-support.h"


int
main(int argc, char *argv[])
{
  /*
   * Set up
   */
  GtkWidget *combo;
  GtkTreeModel *model_from_combo;

  gtk_init(&argc, &argv);

  GtkTreeIter top_level;
  GtkTreeStore *model_store = gtk_tree_store_new(2,
                                                 G_TYPE_INT,
                                                 G_TYPE_STRING);
  gtk_tree_store_append(model_store, &top_level, NULL);
  gtk_tree_store_set(model_store, &top_level,
                     0, 1,
                     1, "Root 1", -1);
  GtkTreeModel *model = GTK_TREE_MODEL(model_store);


  /*
   * Construction
   */
  check(kgtk3_combo_box_new_with_model(NULL) == NULL,
        "should fail when new_with_model receives NULL");

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  check(kgtk3_combo_box_new_with_model((GtkTreeModel *) window) == NULL,
        "should fail when new_with_model receives an invalid object");


  /*
   * Model on construction
   */
  combo = kgtk3_combo_box_new_with_model(model);
  model_from_combo = gtk_combo_box_get_model(GTK_COMBO_BOX(combo));
  check(KGTK3_IS_COMBO_MODEL(model_from_combo),
        "should wrap the model on a KGtk3ComboModel on construction");


  /*
   * Row separator function
   */
  combo = kgtk3_combo_box_new_with_model(model);
  check(gtk_combo_box_get_row_separator_func(GTK_COMBO_BOX(combo)) != NULL,
        "should set row separator function");


  /*
   * End
   */
  return finalize();
}
