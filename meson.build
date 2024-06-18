project('roaring_race_editor', 'cpp')

gtkdep = dependency('gtkmm-4.0')

gnome = import('gnome')

cc_resources = ['main.cc', 'geometry.cc', 'map.cc' , 'editor.cc']

ui_resources = gnome.compile_resources('resources', 'resources/resources.gresources.xml', source_dir : 'resources')

executable('roaring_race_editor', ui_resources, cc_resources, dependencies : gtkdep)
