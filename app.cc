#include "app.h"

#include <exception>
#include <iostream>

#include "window.h"

Application::Application()
    : Gtk::Application("net.roaringmind.roaring_race_editor",
                       Gio::Application::Flags::HANDLES_OPEN) {}

Glib::RefPtr<Application> Application::create() {
  return Glib::make_refptr_for_instance<Application>(new Application());
}

Window* Application::create_appwindow() {
  Window* appwindow = Window::create();

  // Make sure that the application runs for as long this window is still open.
  add_window(*appwindow);

  // A window can be added to an application with Gtk::Application::add_window()
  // or Gtk::Window::set_application(). When all added windows have been hidden
  // or removed, the application stops running (Gtk::Application::run()
  // returns()), unless Gio::Application::hold() has been called.

  // Delete the window when it is hidden.
  appwindow->signal_hide().connect([appwindow]() { delete appwindow; });

  return appwindow;
}

void Application::on_activate() {
  try {
    // The application has been started, so let's show a window.
    Window* appwindow = create_appwindow();
    appwindow->present();
  }
  // If create_appwindow() throws an exception (perhaps from Gtk::Builder),
  // no window has been created, no window has been added to the application,
  // and therefore the application will stop running.
  catch (const Glib::Error& ex) {
    std::cerr << "ExampleApplication::on_activate(): " << ex.what()
              << std::endl;
  } catch (const std::exception& ex) {
    std::cerr << "ExampleApplication::on_activate(): " << ex.what()
              << std::endl;
  }
}