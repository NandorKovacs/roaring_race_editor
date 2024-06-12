#include "editor.h"

#include <gtkmm.h>

#include <iostream>

#include "geometry.h"

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
    : Gtk::DrawingArea(cobject),
      gesture_click(Gtk::GestureClick::create()),
      key_event(Gtk::EventControllerKey::create()) {
  set_draw_func(sigc::mem_fun(*this, &MapEditor::draw));
  gesture_click->signal_pressed().connect(
      sigc::mem_fun(*this, &MapEditor::click));
  add_controller(gesture_click);
}

MapEditor* MapEditor::create(Glib::RefPtr<Gtk::Builder> builder) {
  return Gtk::Builder::get_widget_derived<MapEditor>(builder, "map-editor");
}

void MapEditor::draw(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                     int height) {
  cr->set_line_width(10);
  cr->set_source_rgb(1, 0, 0);
  // cr->move_to(width / 2, height / 2);
  // cr->line_to(width / 2 + 50, height / 2 + 50);

  Circle* c = new Circle(width / 2, height / 2, 100.0);
  c->draw(cr, width, height);
  cr->stroke();
}

void MapEditor::click(gint n_press, gdouble x, gdouble y) {
  std::cout << "clicked" << std::endl;
}

DrawingState::DrawingState() : t(IDLE), state(ToolState::create(t)) {}

ToolState* ToolState::create(Tool t) {
  switch (t) {
    case IDLE:
      return new IdleState();

    case CIRCLE:
      return new CircleState();
  }
}

Tool DrawingState::current_tool() {
  return t;
}

void DrawingState::set_tool(Tool t) {
  // delete state;
  state = ToolState::create(t);
}