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

You can just replace `Gtk3::ComboBox` with `KGtk3::ComboBox`:

```perl
use KGtk3::ComboModel;

my $combo = KGtk3::ComboBox->new_with_model($model)
```

If you need more control, you can wrap the model manually:

```perl
use KGtk3::ComboModel;

my $base_model = Gtk3::TreeStore->new(...)
my $model = KGtk3::ComboModel->new($base_model);

my $combo = Gtk3::ComboBox->new_with_model($model);
my $sep_col = $model->get_separator_column();
$combo->set_row_separator_func(sub {
    my ($model, $iter) = @_;
    return $model->get($iter, $sep_col);
  });
```

For more details, see the [main package documentation](../README.md).
