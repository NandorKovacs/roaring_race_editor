#ifndef ROARING_RACE_APP
#define ROARING_RACE_APP

#include <gtkmm.h>

class Window;

class Application : public Gtk::Application {
 public:
  static Glib::RefPtr<Application> create();

 protected:
  Application();
  // Override default signal handlers:
  void on_activate() override;

 private:
  Window* create_appwindow();
};

#endif /* ROARING_RACE_APP */