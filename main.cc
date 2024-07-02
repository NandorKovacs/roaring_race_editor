#include <gtkmm.h>

#include "editor.h"

int main(int argc, char* argv[]) {
  Glib::RefPtr<Editor> editor = Editor::create();
  return editor->run();
}