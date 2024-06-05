#include <gtkmm.h>

class EditorWindow : public Gtk::ApplicationWindow {
 public:
  EditorWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
  static EditorWindow* create();

 protected:
  Glib::RefPtr<Gtk::Builder> builder;
};

class Editor : public Gtk::Application {
 public: 
  static Glib::RefPtr<Editor> create();

 protected:
  Editor();
  void  on_activate() override;
};