#ifndef DUNGEONMAP_H
#define DUNGEONMAP_H

#include <vector>
#include <iostream>

typedef std::pair<size_t, size_t> Point;

template <typename CellT>
class DungeonMap {

 public:
  typedef CellT CellType;

  DungeonMap(size_t width, size_t height);

  const CellT& GetCell(size_t x, size_t y) const;

  void SetCell(size_t x, size_t y, const CellT& cell);

  const Point GetSize() const;

 private:
  // Store the map as a single dimensional vector of size width * height
  std::vector< CellT > m_cells;

  size_t m_width;
  size_t m_height;

  size_t CalcIndex(size_t x, size_t y) const { return y*m_width + x; }

};

template <typename CellT>
DungeonMap<CellT>::DungeonMap(size_t width, size_t height):
  m_width(width), m_height(height)
{
  m_cells.resize(m_width * m_height, CellT::Closed());
}

template <typename CellT>
const CellT& DungeonMap<CellT>::GetCell(size_t x, size_t y) const
{
  assert(0 <= CalcIndex(x,y));
  assert(CalcIndex(x,y) < m_cells.size());
  return m_cells[CalcIndex(x,y)];
}

template <typename CellT>
void DungeonMap<CellT>::SetCell(size_t x, size_t y, const CellT& cell)
{
  assert(0 <= CalcIndex(x,y));
  assert(CalcIndex(x,y) < m_cells.size());
  m_cells[CalcIndex(x,y)] = cell;
}

template <typename CellT>
const Point DungeonMap<CellT>::GetSize() const
{
  return std::make_pair(m_width, m_height);
}

template<typename CellT>
std::ostream& operator << (std::ostream& os, const DungeonMap<CellT> & map)
{
  const Point size = map.GetSize();
  for(size_t y = 0; y < size.second; ++y) {
    for(size_t x = 0; x < size.first; ++x) {
      os << map.GetCell(x, y);
    }
    os << std::endl;
  }
  return os;
}

#endif
