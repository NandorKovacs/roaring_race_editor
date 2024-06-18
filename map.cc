#include "map.h"

void Map::insert(Geometry* object) { objects.insert(object); };

void Map::remove(Geometry* object) { objects.erase(object); }

std::unordered_set<Geometry*>::iterator Map::begin() { return objects.begin(); }

std::unordered_set<Geometry*>::iterator Map::end() { return objects.end(); }