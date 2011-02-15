#ifndef TERRAIN_H
#define TERRAIN_H

#include <iosfwd>

enum TerrainType {
  OpenFloorT,
  HallwayT,
  HRegionT,
  VRegionT,
  OneT,
  TwoT,
  WallT
};

class Terrain {
 public:

  Terrain();
  Terrain(TerrainType type);
  
  const bool IsOpen() const { return m_type == OpenFloorT || m_type == HallwayT || m_type == OneT || m_type == TwoT; }
  const TerrainType GetType() const { return m_type; }

  static const Terrain Open() { return Terrain(OpenFloorT); }
  static const Terrain One() { return Terrain(OneT); }
  static const Terrain Two() { return Terrain(TwoT); }
  static const Terrain Hallway() { return Terrain(HallwayT); }
  static const Terrain HRegion() { return Terrain(HRegionT); }
  static const Terrain VRegion() { return Terrain(VRegionT); }
  static const Terrain Closed() { return Terrain(WallT); }
 private:
  TerrainType m_type;
};

std::ostream& operator << (std::ostream& os, const Terrain & terrain);



#endif
