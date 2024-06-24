#include "geometry.h"

#include <gtkmm.h>

#include <iostream>

Circle::Circle(double x, double y, double r) : m({x, y}), r(r) {};
Circle::Circle(Point m, double r) : m(m), r(r) {};

void Circle::draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  cr->arc(m.x, m.y, r, 0, 2 * M_PI);
}

Point Point::operator*(double d) { return {x * d, y * d}; }

Point Point::operator/(double d) { return {x / d, y / d}; }

Point Point::operator+(Point p) { return {x + p.x, y + p.y}; }

Point Point::operator-(Point p) { return operator+(p * -1); }

std::ostream& operator<<(std::ostream& os, Point& point) {
  os << point.x << " " << point.y;
  return os;
}