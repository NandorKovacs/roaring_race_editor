#include "geometry.h"

#include <iostream>

#include <gtkmm.h>

Circle::Circle(double x, double y, double r) : m({x, y}), r(r) {};
Circle::Circle(Point m, double r) : m(m), r(r) {};

void Circle::draw(const Cairo::RefPtr<Cairo::Context>& cr, std::function<Point(Point)> view_func) {
  Point screen_pt = view_func(m);
  cr->arc(screen_pt.x, screen_pt.y, r, 0, 2 * M_PI);
}

Point Point::operator*(double d) { return {x * d, y * d}; }

Point Point::operator/(double d) { return {x / d, y / d}; }

Point Point::operator+(Point p) { return {x + p.x, y + p.y}; }

Point Point::operator-(Point p) { return operator+(p * -1); }