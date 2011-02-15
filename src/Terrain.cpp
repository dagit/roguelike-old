#include "Terrain.h"
#include <iostream>

Terrain::Terrain() : m_type(OpenFloorT) {}

Terrain::Terrain(TerrainType type): m_type(type) {}

std::ostream& operator << (std::ostream& os, const Terrain & terrain)
{
  if( terrain.IsOpen() ){
    os << ".";
  } else {
    os << "#";
  }
  
  // This version is good for debugging:
//   switch ( terrain.GetType() ){
//     case OneT:
//       os << "1";
//       break;
//     case TwoT:
//       os << "2";
//       break;
//     case HallwayT:
//       os << ".";
//       break;
//     case OpenFloorT:
//       os << " ";
//       break;
//     case HRegionT:
//       os << "-";
//       break;
//     case VRegionT:
//       os << "|";
//       break;
//     case WallT:
//       os << "#";
//       break;
//     default:
//       assert(false);
//   }
    
  return os;
}
