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
      map(),
      view(),
      tool_state(new SelectState()),
      gesture_click(Gtk::GestureClick::create()),
      gesture_drag(Gtk::GestureDrag::create()) {
  map.insert(new Circle(0, 0, 100.0));

  set_draw_func(sigc::mem_fun(*this, &MapEditor::draw));
  signal_resize().connect([this](double width, double height) {
    this->width = width;
    this->height = height;
  });

  {
    gesture_click->signal_pressed().connect(
        sigc::mem_fun(*this, &MapEditor::click));
    add_controller(gesture_click);
  }  // gesture click
  {
    gesture_drag->signal_drag_begin().connect(
        sigc::mem_fun(*this, &MapEditor::drag_start));
    gesture_drag->signal_drag_update().connect(
        sigc::mem_fun(*this, &MapEditor::drag_update));
    gesture_drag->signal_drag_end().connect(
        sigc::mem_fun(*this, &MapEditor::drag_end));
    add_controller(gesture_drag);
  }
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

  // maybe reimplement with cairo matrixes if they exist?
  // pro: less hacky
  // con: this way, the translation happens in MapView for drawing, and for
  // clicking too
  std::function<Point(Point)> fnc = std::bind(
      &MapView::map_to_screen, &view, std::placeholders::_1, width, height);

  for (auto it = map.begin(); it != map.end(); ++it) {
    (**it).draw(cr, fnc);
  }

  cr->stroke();
}

void MapEditor::click(gint n_press, gdouble x, gdouble y) {
  tool_state->click(view.screen_to_map({x, y}, width, height));
}

void MapEditor::drag_start(gdouble x, gdouble y) {
  std::cout << "start: " << x << " " << y << std::endl;
}

void MapEditor::drag_update(gdouble x, gdouble y) {
  // std::cout << "update: " << x << " " << y << std::endl;
  view.drag_update(view.screen_to_map({x, y}, width, height));
}

void MapEditor::drag_end(gdouble x, gdouble y) {
  std::cout << "end: " << x << " " << y << std::endl;
  view.drag_end(view.screen_to_map({x, y}, width, height));
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
  std::cout << "Select: " << p.x << " " << p.y << std::endl;
}

void CircleState::click(Point p) {
  std::cout << "Circle: " << p.x << " " << p.y << std::endl;
}

MapView::MapView() : translate({0, 0}), zoom(1), delta({0}) {}

Point MapView::map_to_screen(Point p, double width, double height) {
  Point center = {width / 2, height / 2};
  return center - ((p - translate) * zoom);
}

Point MapView::screen_to_map(Point p, double width, double height) {
  Point center = Point{width / 2, height / 2};
  Point res = (center - p) / zoom + translate;
  return {res.x * -1, res.y};
}

void MapView::drag_update(Point p) {
  delta = p;
  std::cout << p.x << " " << p.y << std::endl;
}

void MapView::drag_end(Point p) {
  delta = {0};
  translate = translate + p;
}