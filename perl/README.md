# Perl bindings for KGtk3ComboModel

## Installation

To install the module do the standard

```sh
perl Makefile.PL
make
make test
make install
```

## Dependencies

You'll need KGtk3ComboModel built as a shared library and installed. You'll also need the typelib file. By default, it is built and installed if the gobject-instrospection tools are installed.

## Usage

```perl
my $base_model = Gtk3::TreeStore->new(...)
my $model = KGtk3::ComboModel->new($model);

my $combo = Gtk3::ComboBox->new_with_model($model);
$combo->set_row_separator_func(sub {
    my ($model, $iter) = @_;
    return $model->get($iter, LAST_COLUMN+1);
  });
```

For more details, see the [main package documentation](../README.md).
