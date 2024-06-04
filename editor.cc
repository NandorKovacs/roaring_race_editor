#include "editor.h"

#include <gtkmm.h>

Editor::Editor() : Gtk::Application("net.roaringmind.roaring_race_editor") {}

Glib::RefPtr<Editor> Editor::create() {
  return Glib::make_refptr_for_instance<Editor>(new Editor());
}

void Editor::on_activate() {
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_resource("/resources/ui.xml");
  
}