#include "window.h"

#include <stdexcept>

Window::Window(BaseObjectType* cobject,
               const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::ApplicationWindow(cobject), m_refBuilder(refBuilder) {}

// static
Window* Window::create() {
  // Load the Builder file and instantiate its widgets.
  auto refBuilder = Gtk::Builder::create_from_resource("/resources/ui.xml");

  Window* window =
      Gtk::Builder::get_widget_derived<Window>(refBuilder, "app_window");
  if (!window) {
    throw std::runtime_error("No \"app_window\" object in window.ui");
  }

  return window;
}