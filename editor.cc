#include "editor.h"

#include <gtkmm.h>

#include <cassert>
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

const std::vector<std::pair<std::string, Tool>> BUTTON_IDS = {
    {"tool-select", SELECT}, {"tool-circle", CIRCLE}};

EditorWindow::EditorWindow(BaseObjectType* cobject,
                           const Glib::RefPtr<Gtk::Builder>& builder)
    : Gtk::ApplicationWindow(cobject),
      builder(builder),
      editor{MapEditor::create(builder)},
      tool_buttons({}) {
  auto change_tool_ptr = std::mem_fn(&MapEditor::change_tool);
  for (auto [id, tool] : BUTTON_IDS) {
    auto change_tool_bind = std::bind(change_tool_ptr, editor, tool);
    tool_buttons.push_back(ToolButton::create(builder, id));
    tool_buttons.back()->signal_clicked().connect(change_tool_bind);
  }
}

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
      key_event(Gtk::EventControllerKey::create()),
      tool_state(new SelectState()),
      objects({}),
      ghosts({}) {
  set_draw_func(sigc::mem_fun(*this, &MapEditor::draw));
  gesture_click->signal_pressed().connect(
      sigc::mem_fun(*this, &MapEditor::click));
  add_controller(gesture_click);
}

ToolButton::ToolButton(BaseObjectType* cobject,
                       const Glib::RefPtr<Gtk::Builder>& builder)
    : Gtk::Button(cobject) {}

ToolButton* ToolButton::create(Glib::RefPtr<Gtk::Builder> builder,
                               const Glib::ustring& id) {
  return Gtk::Builder::get_widget_derived<ToolButton>(builder, id);
}

MapEditor* MapEditor::create(Glib::RefPtr<Gtk::Builder> builder) {
  return Gtk::Builder::get_widget_derived<MapEditor>(builder, "map-editor");
}

void MapEditor::draw(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                     int height) {
  cr->set_line_width(10);
  cr->set_source_rgb(1, 0, 0);

  Circle* c = new Circle(width / 2, height / 2, 100.0);
  c->draw(cr, width, height);
  cr->stroke();
}

void MapEditor::click(gint n_press, gdouble x, gdouble y) {
  tool_state->click({x, y});
}

void MapEditor::change_tool(Tool t) {
  delete tool_state;
  switch (t) {
    case SELECT:
      std::cout << "changed to select mode" << std::endl;
      tool_state = new SelectState();
      break;

    case CIRCLE:
      std::cout << "changed to circle mode" << std::endl;
      tool_state = new CircleState();
      break;

    default:
      std::cerr << "unknown tool" << std::endl;
      assert(false);
      break;
  }
}

ToolState::ToolState(Tool t) : t(t) {}
Tool ToolState::type() { return t; }

SelectState::SelectState() : ToolState(SELECT) {}
CircleState::CircleState() : ToolState(CIRCLE) {}

void SelectState::click(Point p) {
  std::cout << "Select: " << p.first << " " << p.second << std::endl;
}

void CircleState::click(Point p) {
  std::cout << "Circle: " << p.first << " " << p.second << std::endl;
}