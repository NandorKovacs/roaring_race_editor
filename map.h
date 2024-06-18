#ifndef ROARING_EDITOR_MAP_H
#define ROARING_EDITOR_MAP_H

#include <unordered_set>

#include "geometry.h"

class Map {
 public:
  void insert(Geometry* object);
  void remove(Geometry* object);
  std::unordered_set<Geometry*>::iterator begin();
  std::unordered_set<Geometry*>::iterator end();

  
 private:
  std::unordered_set<Geometry*> objects;
};

#endif  // ROARING_EDITOR_MAP_H