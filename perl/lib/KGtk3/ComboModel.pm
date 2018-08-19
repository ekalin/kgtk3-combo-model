# Copyright (C) 2018 Eduardo M KALINOWSKI <eduardo@kalinowski.com.br>.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of the University nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

package KGtk3::ComboModel;
use strict;
use warnings;

our $VERSION = '1.2.0';

=head1 NAME

KGtk3::ComboModel - Makes combos with tree models work like in GTK+ 2, allowing item with children to be selected

=head1 SYNOPSYS

You can just replace `Gtk3::ComboBox` with `KGtk3::ComboBox`:

  use Gtk3 -init;
  use KGtk3::ComboModel;

  # Replace Gtk3::ComboBox with this
  my $combo = KGtk3::ComboBox->new_with_model($model)

and use the combo normally.

If you need more control, you can wrap the model manually:

  use Gtk3 -init;
  use KGtk3::ComboModel;

  my $base_model = Gtk3::TreeStore->new(...)
  my $model = KGtk3::ComboModel->new($base_model);

  my $combo = Gtk3::ComboBox->new_with_model($model);
  my $sep_col = $model->get_separator_column();
  $combo->set_row_separator_func(sub {
      my ($model, $iter) = @_;
      return $model->get($iter, $sep_col);
    });

=head1 DESCRIPTION

Gtk+ 3.17.5 has removed the ability to select items in a
GtkComboBox that have children. In Gtk+ 2 an extra item was added
to the combos, allowing the parent item to be selected, but without
those items in Gtk+ 3 items with children cannot be selected with
the mouse.

KGtk3::ComboModel is a wrapper for Gtk3::TreeModel that returns a view
of the wrapped model with extra items just like Gtk+ 2 used to do.

KGtk3::ComboBox is a drop-in replacement for Gtk3::ComboBox that wraps
its model in a KGtk3::ComboModel automatically. In most cases you can
just replace your combos with this subclass and everything should work
normally.

Note that the model used by the combo is not the model supplied in the
constructor or with `->set_model()`, but a `KGtk3::ComboModel` (see
below). For basic operations such as getting data from the current
iter, using the original model should work. But if you want to do
other operations (such as moving the iter around), you should get the
wrapped model (just use `->get_model()`) and use that instead of the
original model.

If necessary, you can wrap your Gtk3::TreeModel in KGtk3::ComboModel with:

  my $wrapped_model = KGtk3::ComboModel->new($model)

and use C<wrapped_model> in your combo. The wrapped model will
include extra itens corresponding to the parent item and a
separator.

An extra boolean column is added after all the columns of the base
model. This column is C<true> for the separator items, and should be
used in a row separator function, something like this:

  $combo->set_row_separator_func(sub {
      my ($model, $iter) = @_;
      return $model->get($iter, LAST_COLUMN+1);
    });

=head1 TECHNICAL NOTE

KGtk3::ComboModel uses C<user_data3> in iterators to store its
internal data. If the base model uses this field, it cannot be
wrapped. None of the standard Gtk TreeModels use that field, so
it's safe to wrap ListStore, TreeStore, TreeModelFilter and
TreeModelSort.

=head1 AUTHOR

Eduardo M KALINOWSKI <eduardo@kalinowski.com.br>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2018 Eduardo M KALINOWSKI <eduardo@kalinowski.com.br>.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of the University nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

=cut

use Glib::Object::Introspection;
Glib::Object::Introspection->setup(
  basename => 'KGtk3ComboModel',
  version => '1.0',
  package => 'KGtk3');


sub get {
  return Gtk3::TreeModel::get(@_);
}


1;
__END__
