#ifndef ROARING_EDITOR_H
#define ROARING_EDITOR_H

#include <gtkmm.h>

#include <unordered_set>

#include "geometry.h"
#include "map.h"

enum Tool {
  SELECT,
  CIRCLE,
};

class ToolState {
 public:
  virtual void click(Point p) = 0;
  Tool type();
  virtual ~ToolState() = default;

 protected:
  ToolState(Tool t);
  Tool t;
};

class SelectState : public ToolState {
 public:
  SelectState();
  void click(Point p);
};

class CircleState : public ToolState {
 public:
  CircleState();
  void click(Point p);
};

class MapView {
 public:
  MapView();

  Point map_to_screen(Point p, double width, double height);
  Point screen_to_map(Point p, double width, double height);

  void drag_start();
  void drag_update(Point p);
  void drag_end(Point p);
  Point get_drag_start();

 private:
  Point translate;
  double zoom;

  Point old;
};

class MapEditor : public Gtk::DrawingArea {
 public:
  MapEditor(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);

  static MapEditor* create(Glib::RefPtr<Gtk::Builder> builder);

  void change_tool(Tool t);

 private:
  Map map;
  MapView view;
  double height, width;


  ToolState* tool_state;

  // gtk signals

  void draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);

  Glib::RefPtr<Gtk::GestureClick> gesture_click;
  void click(gint n_press, gdouble x, gdouble y);

  Glib::RefPtr<Gtk::GestureDrag> gesture_drag;
  void drag_start(double x, double y);
  void drag_update(double x, double y);
  void drag_end(double x, double y);
};

class ToolButton : public Gtk::Button {
 public:
  ToolButton(BaseObjectType* cobject,
             const Glib::RefPtr<Gtk::Builder>& builder);
  static ToolButton* create(Glib::RefPtr<Gtk::Builder> builder,
                            const Glib::ustring& id);
};

class EditorWindow : public Gtk::ApplicationWindow {
 public:
  EditorWindow(BaseObjectType* cobject,
               const Glib::RefPtr<Gtk::Builder>& builder);
  static EditorWindow* create();

 protected:
  Glib::RefPtr<Gtk::Builder> builder;

 private:
  MapEditor* editor;
  std::vector<ToolButton*> tool_buttons;
};

class Editor : public Gtk::Application {
 public:
  static Glib::RefPtr<Editor> create();

 protected:
  Editor();
  void on_activate() override;
};

#endif  // ROARING_EDITOR_H