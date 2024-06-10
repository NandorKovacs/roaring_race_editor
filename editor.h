#ifndef ROARING_EDITOR_H
#define ROARING_EDITOR_H

#include <gtkmm.h>

class MapEditor : public Gtk::DrawingArea {
 public:
  MapEditor(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);

  static MapEditor* create(Glib::RefPtr<Gtk::Builder> builder);

  void draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);

 private:
  Glib::RefPtr<Gtk::GestureClick> gesture_click;
  void click(gint n_press, gdouble x, gdouble y);
  Glib::RefPtr<Gtk::EventControllerKey> key_event;

  // std::vector <
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