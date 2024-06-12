#ifndef ROARING_EDITOR_H
#define ROARING_EDITOR_H

#include <gtkmm.h>
#include "geometry.h"
#include <unordered_set>

typedef std::pair<double, double> Point;

enum Tool {
  IDLE, CIRCLE,
};

class ToolState {
 public:
  static ToolState* create(Tool t);
  virtual void next_click() = 0;
};

class IdleState : public ToolState {
 public:
  void next_click() {};
};

class CircleState : public ToolState {
 public:
  void next_click();
 private:
  std::vector<Point> pts = {};
};

class DrawingState {
 public:
  DrawingState();
  Tool current_tool();
  void set_tool(Tool t);

 private:
  Tool t;
  ToolState* state;
};



class MapEditor : public Gtk::DrawingArea {
 public:
  MapEditor(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);

  static MapEditor* create(Glib::RefPtr<Gtk::Builder> builder);

  void draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);

 private:
  Glib::RefPtr<Gtk::GestureClick> gesture_click;
  void click(gint n_press, gdouble x, gdouble y);
  Glib::RefPtr<Gtk::EventControllerKey> key_event;


  DrawingState state;

  std::unordered_set<Geometry*> objects;
  std::unordered_set<Geometry*> ghosts;
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
};

class Editor : public Gtk::Application {
 public:
  static Glib::RefPtr<Editor> create();

 protected:
  Editor();
  void on_activate() override;
};

#endif // ROARING_EDITOR_H