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

use strict;
use warnings;

package KGtk3::ComboModel::Test;

use Gtk3 -init;


use base qw(Test::Class);
use Test::More;

use KGtk3::ComboModel;


sub setup_model : Test(setup) {
  my $self = shift;

  my $model = Gtk3::TreeStore->new(
   'Glib::Int',
   'Glib::String',
  );
  my $cmodel = KGtk3::ComboModel->new($model);

  my $iter = $model->append(undef);
  $model->set($iter, 0 => 1, 1 => 'Root 1');

  $self->{model}  = $model;
}


sub should_wrap_the_model_on_construction : Test {
  my $self = shift;

  my $combo = KGtk3::ComboBox->new_with_model($self->{model});
  my $model = $combo->get_model();
  ok($model->isa('KGtk3::ComboModel'));
}


sub should_wrap_the_model_via_set_model : Test {
  my $self = shift;

  my $combo = KGtk3::ComboBox->new();
  $combo->set_model($self->{model});
  my $model = $combo->get_model();
  ok($model->isa('KGtk3::ComboModel'));
}


sub should_keep_the_same_wrapped_model_if_set_model_is_called_with_the_same_base_model : Test {
  my $self = shift;

  my $combo = KGtk3::ComboBox->new();
  $combo->set_model($self->{model});
  my $model1 = $combo->get_model();

  $combo->set_model($self->{model});
  my $model2 = $combo->get_model();
  is($model1, $model2);
}


sub should_wrap_the_model_via_property : Test {
  my $self = shift;

  my $combo = KGtk3::ComboBox->new();
  $combo->set('model' => $self->{model});
  my $model = $combo->get_model();
  ok($model->isa('KGtk3::ComboModel'));
}


sub should_return_same_model_via_get_model_and_get_property : Test {
  my $self = shift;

  my $combo = KGtk3::ComboBox->new_with_model($self->{model});
  my $model1 = $combo->get_model();
  my $model2 = $combo->get('model');
  is($model1, $model2);
}


sub should_be_buildable_with_gtk_ui_builder : Test {
  my $ui_xml = <<'ENDXML';
  <interface>
    <object class="KGtk3ComboBox" id="combo">
      <property name="model">model</property>
    </object>
    <object class="GtkTreeStore" id="model">
      <columns>
        <column type="gchar"/>
      </columns>
    </object>
  </interface>
ENDXML

  my $builder = Gtk3::Builder->new_from_string($ui_xml, -1);
  my $combo = $builder->get_object('combo');
  my $model = $combo->get_model();
  ok($model->isa('KGtk3::ComboModel'));
}


Test::Class->runtests;
