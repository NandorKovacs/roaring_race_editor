#include <gtkmm.h>

#include "editor.h"

int main(int argc, char* argv[]) {
  //  auto application = Application::create();

  //  return application->run(argc, argv);
  Glib::RefPtr<Editor> editor = Editor::create();
  return editor->run();
}