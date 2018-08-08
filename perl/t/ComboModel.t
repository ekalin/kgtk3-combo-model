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

  my ($iter, $child);

  $iter = $model->append(undef);
  $model->set($iter, 0 => 1, 1 => 'Root 1');

  $iter = $model->append(undef);
  $model->set($iter, 0 => 10, 1 => 'Root 2');
  $child = $model->append($iter);
  $model->set($child, 0 => 11, 1 => 'Child 2.1');
  $child = $model->append($iter);
  $model->set($child, 0 => 23, 1 => 'Child 2.2');
  $child = $model->append($iter);
  $model->set($child, 0 => 30, 1 => 'Child 2.3');

  $iter = $model->append(undef);
  $model->set($iter, 0 => 5, 1 => 'Root 3');
  $child = $model->append($iter);
  $model->set($child, 0 => 6, 1 => 'Child 3.1');
  my $subchild = $model->append($child);
  $model->set($subchild, 0 => 16, 1 => 'Child 3.1.1');
  $child = $model->append($iter);
  $model->set($child, 0 => 7, 1 => 'Child 3.2');

  $self->{model}  = $model;
  $self->{cmodel} = $cmodel;
}


sub should_return_same_flags : Test {
  my $self = shift;
  is($self->{cmodel}->get_flags(), $self->{model}->get_flags());
}


sub should_return_one_more_column : Test {
  my $self = shift;
  is($self->{cmodel}->get_n_columns(), $self->{model}->get_n_columns() + 1);
}


sub should_return_existing_column_types : Test {
  my $self = shift;

  subtest 'should return existing column types' => sub {
    for my $i (0..1) {
      is($self->{cmodel}->get_column_type($i),
         $self->{model}->get_column_type($i),
         "column $i");
    }
  };
}


sub should_return_boolean_type_for_last_column : Test {
  my $self = shift;
  is($self->{cmodel}->get_column_type(2), 'Glib::Boolean');
}


sub should_return_number_of_root_children : Test {
  my $self = shift;
  is($self->{cmodel}->iter_n_children(undef), 3);
}


sub should_return_no_children_if_original_has_children : Test {
  my $self = shift;

  subtest 'should return no children if original has children' => sub {
    my $root = $self->{cmodel}->get_iter_first();
    ok(!$self->{cmodel}->iter_has_child($root), 'iter_has_child');
    is($self->{cmodel}->iter_n_children($root), 0, 'iter_n_children');
  };
}


sub should_return_two_extra_children_if_original_has_children : Test {
  my $self = shift;

  subtest 'should return two extra children if original has children' => sub {
    my $iter = $self->{cmodel}->get_iter_first();
    $self->{cmodel}->iter_next($iter);

    ok($self->{cmodel}->iter_has_child($iter), 'iter_has_child');
    is($self->{cmodel}->iter_n_children($iter), 5, 'iter_n_children');
  };
}


sub should_return_no_children_for_virtual_items : Test {
  my $self = shift;

  subtest 'should return no children for virtual items' => sub {
    my $top = $self->{cmodel}->iter_nth_child(undef, 1);
    my $level1 = $self->{cmodel}->iter_children($top);

    ok(!$self->{cmodel}->iter_has_child($level1), 'header, iter_has_child');
    is($self->{cmodel}->iter_n_children($level1), 0, 'header, iter_n_children');

    $self->{cmodel}->iter_next($level1);
    ok(!$self->{cmodel}->iter_has_child($level1), 'separator, iter_has_child');
    is($self->{cmodel}->iter_n_children($level1), 0, 'separator, iter_n_children');
  };
}


sub should_return_first_iter_for_children_when_passed_undef : Test {
  my $self = shift;

  my $iter = $self->{cmodel}->iter_children(undef);
  is($self->{cmodel}->get($iter, 1), 'Root 1');
}


sub should_return_top_level_iter_for_nth_child_when_passed_undef : Test {
  my $self = shift;

  my $iter = $self->{cmodel}->iter_nth_child(undef, 2);
  is($self->{cmodel}->get($iter, 1), 'Root 3');
}


sub should_return_false_for_children_when_iter_has_no_children : Test {
  my $self = shift;

  subtest 'should return false for children when iter has no children' => sub {
    my $iter = $self->{cmodel}->get_iter_first();
    ok(!$self->{cmodel}->iter_children($iter), 'iter_children');
    ok(!$self->{cmodel}->iter_nth_child($iter, 1), 'iter_nth_child');
  };
}


sub should_return_false_for_children_when_index_is_invalid : Test {
  my $self = shift;

  subtest 'should return false for children when index is invalid' => sub {
    ok(!$self->{cmodel}->iter_nth_child(undef, 4), 'level 0');

    my $iter = $self->{cmodel}->iter_nth_child(undef, 1);
    ok(!$self->{cmodel}->iter_nth_child($iter, 5), 'level 1');
  };
}


sub should_return_false_for_children_on_virtual_items : Test {
  my $self = shift;

  my $top = $self->{cmodel}->iter_nth_child(undef, 1);
  my $level1 = $self->{cmodel}->iter_children($top);

  ok(!$self->{cmodel}->iter_children($level1));
}


sub should_return_extra_children_with_with_nth_child : Test {
  my $self = shift;

  subtest 'should return extra children with with nth child' => sub {
    my $top_iter = $self->{cmodel}->iter_nth_child(undef, 1);

    my $child = $self->{cmodel}->iter_nth_child($top_iter, 0);
    is($self->{cmodel}->get($child, 0), 10, 'parent header (col 0)');
    is($self->{cmodel}->get($child, 1), 'Root 2', 'parent header (col 1)');

    $child = $self->{cmodel}->iter_nth_child($top_iter, 2);
    is($self->{cmodel}->get($child, 0), 11, 'existing item 1 (col 0)');
    is($self->{cmodel}->get($child, 1), 'Child 2.1', 'existing item 1 (col 1)');

    $child = $self->{cmodel}->iter_nth_child($top_iter, 3);
    is($self->{cmodel}->get($child, 0), 23, 'existing item 2 (col 0)');
    is($self->{cmodel}->get($child, 1), 'Child 2.2', 'existing item 2 (col 2)');
  };
}


sub should_return_extra_children_with_iter_next : Test {
  my $self = shift;

  subtest 'should return extra children' => sub {
    my $top_iter = $self->{cmodel}->iter_nth_child(undef, 1);

    my $child = $self->{cmodel}->iter_children($top_iter);
    is($self->{cmodel}->get($child, 0), 10, 'parent header (col 0)');
    is($self->{cmodel}->get($child, 1), 'Root 2', 'parent header (col 1)');

    $self->{cmodel}->iter_next($child);
    # Separator

    $self->{cmodel}->iter_next($child);
    is($self->{cmodel}->get($child, 0), 11, 'child 1, existing item 1 (col 0)');
    is($self->{cmodel}->get($child, 1), 'Child 2.1', 'existing item 1 (col 1)');

    $self->{cmodel}->iter_next($child);
    is($self->{cmodel}->get($child, 0), 23, 'existing item 2 (col 0)');
    is($self->{cmodel}->get($child, 1), 'Child 2.2', 'existing item 2 (col 1)');

    $self->{cmodel}->iter_next($child);
    is($self->{cmodel}->get($child, 0), 30, 'existing item 3 (col 0)');
    is($self->{cmodel}->get($child, 1), 'Child 2.3', 'existing item 3 (col 0)');

    ok(!$self->{cmodel}->iter_next($child), 'no more children');
  };
}


sub should_add_extra_separator_column : Test {
  my $self = shift;

  subtest 'should add extra separator column' => sub {
    my $iter = $self->{cmodel}->iter_nth_child(undef, 1);
    ok(!$self->{cmodel}->get($iter, 2), 'level 0');

    $iter = $self->{cmodel}->iter_children($iter);
    ok(!$self->{cmodel}->get($iter, 2), 'level 1, parent header');

    $self->{cmodel}->iter_next($iter);
    ok($self->{cmodel}->get($iter, 2), 'level 1, separator');

    $self->{cmodel}->iter_next($iter);
    ok(!$self->{cmodel}->get($iter, 2), 'level 1, existing item' );
  };
}


sub should_work_with_deeper_hierarchies : Test {
  my $self = shift;

  subtest 'should work with deeper hierarquies' => sub {
    my $top_iter = $self->{cmodel}->iter_nth_child(undef, 2);
    my $child = $self->{cmodel}->iter_nth_child($top_iter, 2);

    my $subchild = $self->{cmodel}->iter_children($child);
    is($self->{cmodel}->get($subchild, 1), 'Child 3.1', 'level 2 header');

    $self->{cmodel}->iter_next($subchild);
    ok($self->{cmodel}->get($subchild, 2), 'level 2 separator');

    $subchild = $self->{cmodel}->iter_nth_child($child, 2);
    is($self->{cmodel}->get($subchild, 1), 'Child 3.1.1', 'level 2 regular');

    ok(!$self->{cmodel}->iter_next($subchild), 'no more children');

    ok(!$self->{cmodel}->iter_children($subchild), 'no level 3 children')
  };
}


sub should_return_parent : Test {
  my $self = shift;
  my ($iter, $parent, $child);

  subtest 'should return parent' => sub {
    $iter = $self->{cmodel}->get_iter_first();
    $parent = $self->{cmodel}->iter_parent($iter);
    ok(!$parent, 'no parent for root');

    $iter = $self->{cmodel}->iter_nth_child(undef, 1);
    $child = $self->{cmodel}->iter_nth_child($iter, 3);
    $parent = $self->{cmodel}->iter_parent($child);
    is($self->{cmodel}->get($parent, 1), 'Root 2', 'parent for regular item');

    $child = $self->{cmodel}->iter_nth_child($iter, 0);
    $parent = $self->{cmodel}->iter_parent($child);
    is($self->{cmodel}->get($parent, 1), 'Root 2', 'parent for extra item');
  };
}


sub should_iterate_backwards_in_root : Test {
  my $self = shift;

  subtest 'should iterate backwards in root' => sub {
    my $iter = $self->{cmodel}->iter_nth_child(undef, 2);
    is($self->{cmodel}->get($iter, 1), 'Root 3', 'item 3');

    $self->{cmodel}->iter_previous($iter);
    is($self->{cmodel}->get($iter, 1), 'Root 2', 'item 2');

    $self->{cmodel}->iter_previous($iter);
    is($self->{cmodel}->get($iter, 1), 'Root 1', 'item 1');

    ok(!$self->{cmodel}->iter_previous($iter), 'no more items');
  };
}


sub should_iterate_backwards_in_subtree : Test {
  my $self = shift;

  subtest 'should iterate backwards in subtree' => sub {
    my $top_iter = $self->{cmodel}->iter_nth_child(undef, 1);

    my $child = $self->{cmodel}->iter_nth_child($top_iter, 3);
    is($self->{cmodel}->get($child, 1), 'Child 2.2', 'item 4');

    $self->{cmodel}->iter_previous($child);
    is($self->{cmodel}->get($child, 1), 'Child 2.1', 'item 3');

    $self->{cmodel}->iter_previous($child);
    ok($self->{cmodel}->get($child, 2), 'item 2 (separator)');

    $self->{cmodel}->iter_previous($child);
    is($self->{cmodel}->get($child, 1), 'Root 2', 'item 1 (header)');

    ok(!$self->{cmodel}->iter_previous($child), 'no more items');
  };
}


sub should_return_iter_from_path_level_0 : Test {
  my $self = shift;

  my $path = Gtk3::TreePath->new_from_indices(2);
  my $iter = $self->{cmodel}->get_iter($path);
  is($self->{cmodel}->get($iter, 1), 'Root 3');
}


sub should_return_iter_from_path_level_1_regular : Test {
  my $self = shift;

  my $path = Gtk3::TreePath->new_from_indices(1, 2);
  my $iter = $self->{cmodel}->get_iter($path);
  is($self->{cmodel}->get($iter, 1), 'Child 2.1');
}


sub should_return_iter_from_path_level_1_virtual : Test {
  my $self = shift;

  subtest 'should return iter from path level 1 virtual' => sub {
    my $path = Gtk3::TreePath->new_from_indices(1, 0);
    my $iter = $self->{cmodel}->get_iter($path);
    is($self->{cmodel}->get($iter, 1), 'Root 2', 'get_iter');

    $self->{cmodel}->iter_next($iter);
    ok($self->{cmodel}->get($iter, 2), 'iter_next');
  };
}


sub should_return_iter_from_path_level_2_regular : Test {
  my $self = shift;

  my $path = Gtk3::TreePath->new_from_indices(2, 2, 2);
  my $iter = $self->{cmodel}->get_iter($path);
  is($self->{cmodel}->get($iter, 1), 'Child 3.1.1');
}


sub should_return_iter_from_path_level_2_virtual : Test {
  my $self = shift;

  subtest 'should return iter from path level 2 virtual' => sub {
    my $path = Gtk3::TreePath->new_from_indices(2, 2, 1);
    my $iter = $self->{cmodel}->get_iter($path);
    ok($self->{cmodel}->get($iter, 2), 'get_iter');

    $self->{cmodel}->iter_previous($iter);
    is($self->{cmodel}->get($iter, 1), 'Child 3.1', 'iter_previous');
  };
}


sub should_return_false_for_non_existing_path_level_0 : Test {
  my $self = shift;

  my $path = Gtk3::TreePath->new_from_indices(4);
  my $iter = $self->{cmodel}->get_iter($path);
  ok(!$iter);
}


sub should_return_false_for_non_existing_path_level_1 : Test {
  my $self = shift;

  my $path = Gtk3::TreePath->new_from_indices(1, 10);
  my $iter = $self->{cmodel}->get_iter($path);
  ok(!$iter);
}


sub should_return_path_from_iter_level_0 : Test {
  my $self = shift;

  my $level0 = $self->{cmodel}->get_iter_first();
  my $path = $self->{cmodel}->get_path($level0);
  is($path->to_string(), '0');
}


sub should_return_path_from_iter_level_1 : Test {
  my $self = shift;

  subtest 'should return path from iter level 1' => sub {
    my $level0 = $self->{cmodel}->iter_nth_child(undef, 1);
    my $level1 = $self->{cmodel}->iter_nth_child($level0, 2);

    my $path = $self->{cmodel}->get_path($level1);
    is($path->to_string(), '1:2', 'regular');

    $self->{cmodel}->iter_previous($level1);
    $path = $self->{cmodel}->get_path($level1);
    is($path->to_string(), '1:1', 'separator');
  };
}


sub should_return_path_from_iter_level_2 : Test {
  my $self = shift;

  subtest 'should return path from iter level 2' => sub {
    my $level0 = $self->{cmodel}->iter_nth_child(undef, 2);
    my $level1 = $self->{cmodel}->iter_nth_child($level0, 2);
    my $level2 = $self->{cmodel}->iter_nth_child($level1, 1);

    my $path = $self->{cmodel}->get_path($level2);
    is($path->to_string(), '2:2:1', 'separator');

    $self->{cmodel}->iter_next($level2);
    $path = $self->{cmodel}->get_path($level2);
    is($path->to_string(), '2:2:2', 'regular');
  };
}



Test::Class->runtests;
