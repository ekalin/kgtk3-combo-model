use ExtUtils::MakeMaker;

WriteMakefile(
  NAME         => 'KGtk3::ComboModel',
  VERSION_FROM => 'lib/KGtk3/ComboModel.pm',
  PREREQ_PM    => { 'Gtk3' => 0 },
  AUTHOR       => 'Eduardo M Kalinowski <eduardo@kalinowski.com.br>',
);
