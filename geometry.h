#ifndef ROARING_EDITOR_GEOMETRY_H
#define ROARING_EDITOR_GEOMETRY_H

#include <gtkmm.h>
#include <unordered_set>
#include <functional>

struct Point {
  double x, y;

  Point (double x, double y) : x(x), y(y) {}
  Point(double d) : x(d), y(d) {}

  Point operator*(double d);
  Point operator/(double d);
  Point operator+(Point p);
  Point operator-(Point p);
  friend std::ostream& operator<<(std::ostream& os, Point& point);
};


class Geometry {
 public:
  virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, std::function<Point(Point)> view_func) = 0;
};

class Circle : public Geometry {
 public:
  Circle(double x, double y, double r);
  Circle(Point m, double r);
  void draw(const Cairo::RefPtr<Cairo::Context>& cr, std::function<Point(Point)> view_func);
 private:
  Point m; 
  double r;
};

#endif  // ROARING_EDITOR_GEOMETRY_H