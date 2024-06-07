#include "editor.h"

#include <iostream>
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
    : Gtk::ApplicationWindow(cobject), builder(builder) {
  editor = MapEditor::create(builder);
}

EditorWindow* EditorWindow::create() {
  Glib::RefPtr<Gtk::Builder> new_builder =
      Gtk::Builder::create_from_resource("/resources/ui.xml");

  return Gtk::Builder::get_widget_derived<EditorWindow>(new_builder,
                                                        "roaring_race_editor");
}

MapEditor::MapEditor(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder) : Gtk::DrawingArea(cobject) {  
  set_draw_func(sigc::mem_fun(*this, &MapEditor::draw));
}

MapEditor* MapEditor::create(Glib::RefPtr<Gtk::Builder> builder) {
  return Gtk::Builder::get_widget_derived<MapEditor>(builder, "map-editor");
}

void MapEditor::draw(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                     int height) {
  cr->set_line_width(10);
  cr->set_source_rgb(1,0,0);
  cr->move_to(width/2, height/2);
  cr->line_to(width/2 + 50, height/2 + 50);
  cr->stroke();
  std::cout << "draw" << std::endl;
}