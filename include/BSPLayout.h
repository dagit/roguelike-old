#ifndef BSPLAYOUT_H
#define BSPLAYOUT_H

#include <cassert>
#include <iostream>
#include "Random.h"

typedef std::pair<size_t, size_t> Point;
template<typename T1, typename T2>
std::ostream& operator << (std::ostream& os, const std::pair<T1,T2>& p);

// Provides a binary space partition layout
// Recursively divides the region either horizontally or vertically
// until the region is small enough, then rooms of random size are
// created in the regions and finally the regions are linked on the
// return of the recursive call.
template <typename MapT>
class BSPLayout {
public:
  BSPLayout(size_t minWidth = 10, size_t minHeight = 10)
    : m_minRoomWidth(minWidth), m_minRoomHeight(minHeight) {}

  void Build(MapT & map);
  
private:
  void split(MapT & map, const Point& upperLeft, const Point& lowerRight);
  void mark(MapT & map, const Point& upperLeft, const Point& lowerRight);
  void connect(MapT & map, bool connectHorizontally, 
               const std::pair< Point, Point> & firstRegion, 
               const std::pair< Point, Point > & secondRegion);
  const Point span(size_t low, size_t high, size_t min) const;

  Random m_rand;
  
  size_t m_minRoomWidth;
  size_t m_minRoomHeight;
};

template<typename MapT>
void BSPLayout<MapT>::Build(MapT & map)
{
  Point maxSize = map.GetSize();
  split(map, std::make_pair(1, 1), std::make_pair(maxSize.first - 1, maxSize.second - 1));
}

template<typename MapT>
void BSPLayout<MapT>::split(MapT & map, const Point& upperLeft, const Point& lowerRight)
{

  // Check the region sizes
  const size_t width  = lowerRight.first  - upperLeft.first;
  const size_t height = lowerRight.second - upperLeft.second;

  // Check if we should split the region or mark a room
  // TODO: add a way for this to stop randomly where the chance is
  // inversly proportional to the area of the room
  const double area = static_cast<double>(width * height);
  const int minArea = m_minRoomHeight * m_minRoomWidth;
  // splitPercent goes to 0 as area decreases to minArea
  const int splitPercent = 100 - static_cast<int>(minArea * 100.0 / area);
  const bool splitIt = m_rand.Next(0, 100) < splitPercent;

  if ( !splitIt ) {
    //std::cout << "split(map, " << firstRegion.first << ", " << firstRegion.second << ");" << std::endl;
    // TODO: connect this room to something
    mark(map, upperLeft, lowerRight);
    return;
  }

  // Pick either horizontal or vertical by weighting according to shape of the room
  // a wide room should split vertically and a tall room usually split horizontally
  // in order for things to look good
  
  const bool isHorizontal = width < static_cast<size_t>(m_rand.Next(0, width + height));

  // Pick a point that is +/- 5% of the middle
  const double percent = m_rand.Next(45, 55)/100.0;

  const size_t rangeStart = isHorizontal ? upperLeft.second : upperLeft.first;
  const size_t rangeEnd   = isHorizontal ? lowerRight.second : lowerRight.first;

  const size_t p = static_cast<size_t>(percent*(rangeEnd - rangeStart)) + rangeStart;

  assert( rangeStart < p && p < rangeEnd );
  
  // Calculate the new regions
  std::pair<Point, Point> firstRegion;
  std::pair<Point, Point> secondRegion;
  if( isHorizontal ) {
    firstRegion  = std::make_pair( upperLeft, std::make_pair(lowerRight.first, p));
    secondRegion = std::make_pair( std::make_pair(upperLeft.first, p), lowerRight);
  } else {
    firstRegion  = std::make_pair( upperLeft, std::make_pair(p, lowerRight.second));
    secondRegion = std::make_pair( std::make_pair(p, upperLeft.second), lowerRight);
  }

  // Check the region sizes
  const size_t firstWidth  = firstRegion.second.first  - firstRegion.first.first;
  const size_t firstHeight = firstRegion.second.second - firstRegion.first.second;

  const size_t secondWidth  = secondRegion.second.first  - secondRegion.first.first;
  const size_t secondHeight = secondRegion.second.second - secondRegion.first.second;

  if ( firstWidth > m_minRoomWidth && firstHeight > m_minRoomHeight && 
       secondWidth > m_minRoomWidth && secondHeight > m_minRoomHeight) {
    //std::cout << "split(map, " << firstRegion.first << ", " << firstRegion.second << ");" << std::endl;
    split(map, firstRegion.first, firstRegion.second);
    split(map, secondRegion.first, secondRegion.second);
    // Now that we have split the regions, we should connect rooms
    connect(map, !isHorizontal, firstRegion, secondRegion);
  } else {
    // TODO: connect this room to something
    mark(map, upperLeft, lowerRight);
    return;
  }
  
}

template<typename MapT>
void BSPLayout<MapT>::mark(MapT & map, const Point& upperLeft, const Point& lowerRight)
{
  // Pick a random region that fills up a percent of each the width/height of the region

  const Point width( span(upperLeft.first, lowerRight.first, m_minRoomWidth) );
  const Point height( span(upperLeft.second, lowerRight.second, m_minRoomHeight) );

  {
    size_t y = upperLeft.second;
    for( size_t x = upperLeft.first; x < lowerRight.first; ++x){
      
      if( !map.GetCell(x,y).IsOpen() ){
        map.SetCell(x, y, MapT::CellType::HRegion());
      }
    }
  }

  {
    size_t y = lowerRight.second;
    for( size_t x = upperLeft.first; x < lowerRight.first; ++x){
      
      if( !map.GetCell(x,y).IsOpen() ){
        map.SetCell(x, y, MapT::CellType::HRegion());
      }
    }
  }

  {
    size_t x = upperLeft.first;
    for( size_t y = upperLeft.second; y < lowerRight.second; ++y){
      
      if( !map.GetCell(x,y).IsOpen() ){
        map.SetCell(x, y, MapT::CellType::VRegion());
      }
    }
  }

  {
    size_t x = lowerRight.first;
    for( size_t y = upperLeft.second; y < lowerRight.second; ++y){
      
      if( !map.GetCell(x,y).IsOpen() ){
        map.SetCell(x, y, MapT::CellType::VRegion());
      }
    }
  }

  for( size_t x = width.first; x < width.second; ++x){
    for( size_t y = height.first; y < height.second; ++y){
      map.SetCell(x,y, MapT::CellType::Open());
    }
  }

}

template<typename MapT>
const Point BSPLayout<MapT>::span(size_t low, size_t high, size_t min) const
{
  assert(low < high);

  size_t p1, p2;
  if((high - low) > min)
  {
    do {
      p1 = m_rand.Next( low, high );
      p2 = m_rand.Next( low, high );
    } while ( std::abs(static_cast<int>(p1 - p2)) < min );
  } else {
    p1 = low;
    p2 = high;
  }
  
  return std::make_pair( std::min(p1,p2), std::max(p1,p2) );
}

template<typename MapT>
const Point BSPLayout<MapT>::ScanForPoint(size_t start, size_t end)
{
  size_t y1;
  Point p1;
  bool foundIt = false;
  // We'd like to avoid potentially infinite loops here.
  // So we try to pick a distinct point each time and stop when there are
  // no more points in the range
  std::vector< size_t > points;
  points.reserve( abs(static_cast<int>(end - start) );
  do {
    y1 = m_rand.Next( std::min(start, end), std::max(start, end) );
    
    // Calculate connection points
    for( size_t x = firstRegion.second.first - 1; x > firstRegion.first.first; --x){
      if( map.GetCell(x,y1).IsOpen() ){
        p1 = std::make_pair(x,y1);
        foundIt = true;
        break;
      }
    }
  } while (!foundIt);
  return p1;
}

template<typename MapT>
void BSPLayout<MapT>::connect(MapT & map, bool connectHorizontally, 
                              const std::pair< Point, Point> & firstRegion,
                              const std::pair< Point, Point > & secondRegion )
{
  // Make a path/corridor/hallway connecting to any rooms in the two regions
  if( connectHorizontally ) { // attempt the connection in a horizontal direction
    // Pick a random placement along the wall
    size_t y1;
    Point p1;
    bool foundIt = false;
    do {
      y1 = m_rand.Next( firstRegion.first.second, firstRegion.second.second );

      // Calculate connection points
      for( size_t x = firstRegion.second.first - 1; x > firstRegion.first.first; --x){
        if( map.GetCell(x,y1).IsOpen() ){
          p1 = std::make_pair(x,y1);
          foundIt = true;
          break;
        }
      }
    } while (!foundIt);

    size_t y2;
    Point p2;
    foundIt = false;
    do {
      y2 = m_rand.Next( secondRegion.first.second, secondRegion.second.second );

      for( size_t x = secondRegion.first.first; x < secondRegion.second.first; ++x){
        if( map.GetCell(x,y2).IsOpen() ){
          p2 = std::make_pair(x,y2);
          foundIt = true;
          break;
        }
      }
    } while (!foundIt);

    size_t jog;
    if( p2.first - p1.first > 4)
      jog = m_rand.Next(2, p2.first - p1.first - 1);
    else
      jog = (p2.first - p1.first) / 2;

    // jog over a bit to make a connection
    for( size_t x = p1.first+1; x <= p1.first + jog; ++x){
      map.SetCell(x, y1, MapT::CellType::Hallway());
    }
    // Go up (or down) to y2
    const size_t highY = std::max(y1,y2);
    const size_t lowY  = std::min(y1,y2);
    for( size_t y = lowY; y < highY; ++y){
      map.SetCell(p1.first + jog, y, MapT::CellType::Hallway());
    }
    // Jog the rest of the way
    for( size_t x = p1.first + jog; x < p2.first; ++x){
      map.SetCell(x, y2, MapT::CellType::Hallway());
    }
    map.SetCell(p1.first, p1.second, MapT::CellType::One());
    map.SetCell(p2.first, p2.second, MapT::CellType::Two());


  } else {

    // Pick a random placement along the wall
    size_t x1;
    Point p1;
    bool foundIt = false;
    do {
      x1 = m_rand.Next( firstRegion.first.first, firstRegion.second.first );

      // Calculate connection points
      for( size_t y = firstRegion.second.second - 1; y > firstRegion.first.second; --y){
        if( map.GetCell(x1,y).IsOpen() ){
          p1 = std::make_pair(x1,y);
          foundIt = true;
          break;
        }
      }
    } while (!foundIt);

    size_t x2;
    Point p2;
    foundIt = false;
    do {
      x2 = m_rand.Next( secondRegion.first.first, secondRegion.second.first );

      for( size_t y = secondRegion.first.second; y < secondRegion.second.second; ++y){
        if( map.GetCell(x2,y).IsOpen() ){
          p2 = std::make_pair(x2,y);
          foundIt = true;
          break;
        }
      }
    } while (!foundIt);

    size_t jog;
    if( p2.second - p1.second> 4)
      jog = m_rand.Next(2, p2.second - p1.second - 1);
    else
      jog = (p2.second - p1.second) / 2;

    // jog over a bit to make a connection
    for( size_t y = p1.second+1; y <= p1.second + jog; ++y){
      map.SetCell(x1, y, MapT::CellType::Hallway());
    }
    // Go over to x2
    const size_t highX = std::max(x1,x2);
    const size_t lowX  = std::min(x1,x2);
    for( size_t x = lowX; x < highX; ++x){
      map.SetCell(x, p1.second + jog, MapT::CellType::Hallway());
    }
    // Jog the rest of the way
    for( size_t y = p1.second + jog; y < p2.second; ++y){
      map.SetCell(x2, y, MapT::CellType::Hallway());
    }

    map.SetCell(p1.first, p1.second, MapT::CellType::One());
    map.SetCell(p2.first, p2.second, MapT::CellType::Two());

  }


}

template<typename T1, typename T2>
std::ostream& operator << (std::ostream& os, const std::pair<T1,T2>& p)
{
  os << "(" << p.first << "," << p.second << ")";
  return os;
}

#endif
