#include <gtkmm.h>

class Editor : public Gtk::Application {
 public: 
  static Glib::RefPtr<Editor> create();

 protected:
  Editor() {
    
  };
};


int main(int argc, char* argv[])
{
//  auto application = Application::create();

//  return application->run(argc, argv);
}