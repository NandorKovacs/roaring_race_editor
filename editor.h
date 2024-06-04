#include <gtkmm.h>

class Editor : public Gtk::Application {
 public: 
  static Glib::RefPtr<Editor> create();

 protected:
  Editor();
  void  on_activate() override;
};