#include "editor.h"

#include <gtkmm.h>

Editor::Editor() : Gtk::Application("net.roaringmind.roaring_race_editor") {}

Glib::RefPtr<Editor> Editor::create() {
  return Glib::make_refptr_for_instance<Editor>(new Editor());
}

void Editor::on_activate() {
  EditorWindow* window = EditorWindow::create();
  add_window(*window);
  window->signal_hide().connect([window]() { delete window; });
  window->present();
}

EditorWindow::EditorWindow(BaseObjectType* cobject,
                           const Glib::RefPtr<Gtk::Builder>& builder)
    : Gtk::ApplicationWindow(cobject), builder(builder) {}

EditorWindow* EditorWindow::create() {
  Glib::RefPtr<Gtk::Builder> new_builder = Gtk::Builder::create_from_resource("/resources/ui.xml");

  return Gtk::Builder::get_widget_derived<EditorWindow>(new_builder, "roaring_race_editor");
}