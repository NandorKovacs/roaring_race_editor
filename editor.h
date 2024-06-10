#include <gtkmm.h>

class MapEditor;

class EditorEventHandler {
 public:
  EditorEventHandler();

  void attach(MapEditor* editor);

 private:
  void click(gint n_press, gdouble x, gdouble y);
  bool key_press(guint keyval, guint, Gdk::ModifierType state);
  Glib::RefPtr<Gtk::GestureClick> gesture_click;
  Glib::RefPtr<Gtk::EventControllerKey> key_event;
};

class MapEditor : public Gtk::DrawingArea {
 public: 
  MapEditor(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder); 

  static MapEditor* create(Glib::RefPtr<Gtk::Builder> builder);

  void draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
 private:
  EditorEventHandler* handler;
};

class EditorWindow : public Gtk::ApplicationWindow {
 public:
  EditorWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
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
  void  on_activate() override;
};