#include "editor.h"

#include <gtkmm.h>

#include <cassert>
#include <iostream>

#include "geometry.h"

// Editor

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

// EditorWindow

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

// MapEditor

MapEditor::MapEditor(BaseObjectType* cobject,
                     const Glib::RefPtr<Gtk::Builder>& builder)
    : Gtk::DrawingArea(cobject),
      map(),
      view(),
      tool_state(new SelectState()),
      gesture_drag(Gtk::GestureDrag::create()) {
  map.insert(new Circle(0, 0, 100.0));

  set_draw_func(sigc::mem_fun(*this, &MapEditor::draw));
  signal_resize().connect([this](double width, double height) {
    this->width = width;
    this->height = height;
  });

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

MapEditor* MapEditor::create(Glib::RefPtr<Gtk::Builder> builder) {
  return Gtk::Builder::get_widget_derived<MapEditor>(builder, "map-editor");
}

void MapEditor::draw(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                     int height) {
  cr->set_line_width(10);
  cr->set_source_rgb(1, 0, 0);

  Cairo::Matrix mx = cr->get_matrix();
  Point translate = view.get_translate();
  double zoom = view.get_zoom();
  mx.translate(translate.x + width/2, translate.y + height / 2);
  mx.scale(zoom, zoom);
  cr->set_matrix(mx);

  for (auto it = map.begin(); it != map.end(); ++it) {
    (**it).draw(cr);
  }

  cr->stroke();
}

void MapEditor::click(gint n_press, gdouble x, gdouble y) {
  tool_state->click(view.screen_to_map({x, y}, width, height));
}

void MapEditor::drag_start(gdouble x, gdouble y) {
  view.drag_start();
}

void MapEditor::drag_update(gdouble x, gdouble y) {
  view.drag_update({x,y});
  queue_draw();
}

void MapEditor::drag_end(gdouble x, gdouble y) {
  view.drag_end({x,y});
  queue_draw();
  if (std::sqrt(x * x + y + y) < 10) {
    Point old = view.get_drag_start();
    click(1, old.x, old.y);
  }
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

// ToolButton

ToolButton::ToolButton(BaseObjectType* cobject,
                       const Glib::RefPtr<Gtk::Builder>& builder)
    : Gtk::Button(cobject) {}

ToolButton* ToolButton::create(Glib::RefPtr<Gtk::Builder> builder,
                               const Glib::ustring& id) {
  return Gtk::Builder::get_widget_derived<ToolButton>(builder, id);
}

// ToolState

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

// MapView

MapView::MapView() : translate({0, 0}), zoom(1), old({0}) {}

Point MapView::map_to_screen(Point p, double width, double height) {
  Point center = {width / 2, height / 2};
  return center - ((p - translate) * zoom);
}

Point MapView::screen_to_map(Point p, double width, double height) {
  Point center = Point{width / 2, height / 2};
  Point res = (center - p) / zoom + translate;
  return {res.x * -1, res.y};
}

void MapView::drag_start() {
  old = translate;
}

void MapView::drag_update(Point p) {
  translate = old + p / zoom;
}

void MapView::drag_end(Point p) {
  translate = old + p / zoom;
  old = {0};
}

Point MapView::get_drag_start() {
  return old;
}

Point MapView::get_translate() {
  return translate;
}

double MapView::get_zoom() {
  return zoom;
}