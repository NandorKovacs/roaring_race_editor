#ifndef ROARING_EDITOR_H
#define ROARING_EDITOR_H

#include <gtkmm.h>

#include <unordered_set>

#include "geometry.h"

typedef std::pair<double, double> Point;

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

class MapEditor : public Gtk::DrawingArea {
 public:
  MapEditor(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);

  static MapEditor* create(Glib::RefPtr<Gtk::Builder> builder);

  void draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);

  void change_tool(Tool t);

 private:
  Glib::RefPtr<Gtk::GestureClick> gesture_click;
  void click(gint n_press, gdouble x, gdouble y);
  Glib::RefPtr<Gtk::EventControllerKey> key_event;

  ToolState* tool_state;

  std::unordered_set<Geometry*> objects;
  std::unordered_set<Geometry*> ghosts;
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