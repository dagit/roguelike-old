#include <iostream>
#include "Dungeon.h"

Dungeon::Dungeon(size_t width, size_t height)
  : m_map(width, height) {}

DungeonMap< Terrain >& Dungeon::GetMap()
{
  return m_map;
}

const DungeonMap< Terrain >& Dungeon::GetMap() const
{
  return m_map;
}

std::ostream& operator << (std::ostream& os, const Dungeon & dungeon)
{
  os << dungeon.GetMap();
  return os;
}
