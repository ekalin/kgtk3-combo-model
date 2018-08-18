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
#include <stdio.h>

#include <kgtk3-combo-model.h>


int errors = 0;
void check(int result, const char *msg)
{
  printf("%s: ", msg);
  if (result) {
    printf("ok\n");
  } else {
    printf("FAILED\n");
    ++errors;
  }
}

void check_col_str(GtkTreeModel *model, GtkTreeIter *iter, const char *expected, const char *msg)
{
  gchar *value;
  gtk_tree_model_get(model, iter, 1, &value, -1);
  check(strcmp(expected, value) == 0, msg);
  g_free(value);
}

void check_col_bool(GtkTreeModel *model, GtkTreeIter *iter, gboolean expected, const char *msg)
{
  gboolean value;
  gtk_tree_model_get(model, iter, 2, &value, -1);
  check(expected == value, msg);
}

int finalize()
{
  if (errors) {
    printf("%d checks FAILED\n", errors);
  } else {
    printf("all checks passed\n");
  }
  return errors;
}


GtkTreeStore *create_model()
{
  GtkTreeIter top_level, child, subchild;

  GtkTreeStore *model_store = gtk_tree_store_new(2,
                                                 G_TYPE_INT,
                                                 G_TYPE_STRING);

  gtk_tree_store_append(model_store, &top_level, NULL);
  gtk_tree_store_set(model_store, &top_level,
                     0, 1,
                     1, "Root 1", -1);

  gtk_tree_store_append(model_store, &top_level, NULL);
  gtk_tree_store_set(model_store, &top_level,
                     0, 10,
                     1, "Root 2", -1);
  gtk_tree_store_append(model_store, &child, &top_level);
  gtk_tree_store_set(model_store, &child,
                     0, 11,
                     1, "Child 2.1", -1);
  gtk_tree_store_append(model_store, &child, &top_level);
  gtk_tree_store_set(model_store, &child,
                     0, 23,
                     1, "Child 2.2", -1);
  gtk_tree_store_append(model_store, &child, &top_level);
  gtk_tree_store_set(model_store, &child,
                     0, 30,
                     1, "Child 2.3", -1);

  gtk_tree_store_append(model_store, &top_level, NULL);
  gtk_tree_store_set(model_store, &top_level,
                     0, 5,
                     1, "Root 3", -1);
  gtk_tree_store_append(model_store, &child, &top_level);
  gtk_tree_store_set(model_store, &child,
                     0, 6,
                     1, "Child 3.1", -1);
  gtk_tree_store_append(model_store, &subchild, &child);
  gtk_tree_store_set(model_store, &subchild,
                     0, 16,
                     1, "Child 3.1.1", -1);
  gtk_tree_store_append(model_store, &child, &top_level);
  gtk_tree_store_set(model_store, &child,
                     0, 7,
                     1, "Child 3.2", -1);

  return model_store;
}

