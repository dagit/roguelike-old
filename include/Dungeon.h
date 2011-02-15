#ifndef DUNGEON_H
#define DUNGEON_H

#include <iosfwd>

#include <DungeonMap.h>
#include <Terrain.h>

class Dungeon {
 public:
  typedef DungeonMap< Terrain > MapT;
  Dungeon(size_t width, size_t height);

  MapT& GetMap();
  const MapT& GetMap() const;

 private:
  MapT m_map;
};

std::ostream& operator << (std::ostream& os, const Dungeon & dungeon);

#endif DUNGEON_H
