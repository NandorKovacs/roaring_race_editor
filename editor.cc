#include "editor.h"

#include <gtkmm.h>

#include <iostream>

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
    : Gtk::ApplicationWindow(cobject),
      builder(builder),
      editor{MapEditor::create(builder)} {}

EditorWindow* EditorWindow::create() {
  Glib::RefPtr<Gtk::Builder> new_builder =
      Gtk::Builder::create_from_resource("/resources/ui.xml");

  return Gtk::Builder::get_widget_derived<EditorWindow>(new_builder,
                                                        "roaring_race_editor");
}

MapEditor::MapEditor(BaseObjectType* cobject,
                     const Glib::RefPtr<Gtk::Builder>& builder)
    : Gtk::DrawingArea(cobject), handler() {
  set_draw_func(sigc::mem_fun(*this, &MapEditor::draw));
  // crashes here
  handler->attach(this);
}

MapEditor* MapEditor::create(Glib::RefPtr<Gtk::Builder> builder) {
  return Gtk::Builder::get_widget_derived<MapEditor>(builder, "map-editor");
}

void MapEditor::draw(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                     int height) {
  cr->set_line_width(10);
  cr->set_source_rgb(1, 0, 0);
  cr->move_to(width / 2, height / 2);
  cr->line_to(width / 2 + 50, height / 2 + 50);
  cr->stroke();
  std::cout << "draw " << width << " " << height << std::endl;
}

EditorEventHandler::EditorEventHandler()
    : gesture_click(Gtk::GestureClick::create()),
      key_event(Gtk::EventControllerKey::create()) {
  gesture_click->signal_pressed().connect(sigc::mem_fun(*this, &EditorEventHandler::click));
  key_event->signal_key_pressed().connect(sigc::mem_fun(*this, &EditorEventHandler::key_press), false);
}

void EditorEventHandler::attach(MapEditor* editor) {
  editor->add_controller(gesture_click);
  editor->add_controller(key_event);
}

void EditorEventHandler::click(gint n_press, gdouble x, gdouble y) {
  std::cout << "haha clicked" << std::endl;
}

bool EditorEventHandler::key_press(guint keyval, guint, Gdk::ModifierType state) {
  std::cout << "haha pressed" << std::endl;
  return true;
}