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
#ifndef __KGTK3_COMBO_BOX_H__
#define __KGTK3_COMBO_BOX_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KGTK3_TYPE_COMBO_BOX (kgtk3_combo_box_get_type())
G_DECLARE_FINAL_TYPE(KGtk3ComboBox, kgtk3_combo_box, KGTK3, COMBO_BOX, GtkComboBox)

GtkWidget *kgtk3_combo_box_new();
GtkWidget *kgtk3_combo_box_new_with_model(GtkTreeModel *model);
GtkWidget *kgtk3_combo_box_new_with_entry();
GtkWidget *kgtk3_combo_box_new_with_model_and_entry(GtkTreeModel *model);

void kgtk3_combo_box_set_model(KGtk3ComboBox *combo, GtkTreeModel *model);

G_END_DECLS

#endif /* __KGTK3_COMBO_BOX_H__ */
