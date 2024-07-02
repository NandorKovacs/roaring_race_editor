#ifndef ROARING_EDITOR_GEOMETRY_H
#define ROARING_EDITOR_GEOMETRY_H
#include <gtkmm.h>

class Geometry {
 public:
  virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                    int height) = 0;
};

class Circle : public Geometry {
 public:
  Circle(double x, double y, double r);
  void draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
 private:
  double x, y, r;
};

#endif  // ROARING_EDITOR_GEOMETRY_H