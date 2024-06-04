#ifndef ROARING_RACE_WINDOW
#define ROARING_RACE_WINDOW

#include <gtkmm.h>

class Window : public Gtk::ApplicationWindow {
 public:
  Window(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);

  static Window* create();

 protected:
  Glib::RefPtr<Gtk::Builder> m_refBuilder;
};

#endif /* ROARING_RACE_WINDOW */