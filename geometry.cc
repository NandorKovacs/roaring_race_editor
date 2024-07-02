#include "geometry.h"

#include <gtkmm.h>

Circle::Circle(double x, double y, double r) : x(x), y(y), r(r){};

void Circle::draw(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                  int height) {
  cr->arc(x, y, r, 0, 2 * M_PI);
}