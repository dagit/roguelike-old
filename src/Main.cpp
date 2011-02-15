#include <iostream>
#include <cstring>

#include "Dungeon.h"
#include "DungeonGenerator.h"
#include "BSPLayout.h"
#include "Random.h"

int main(int argc, char ** argv){
  
  int width = 190;
  int height = 41;
  int minWidth = 10;
  int minHeight = 10;

  if (argc == 5) {
    width = atoi(argv[1]);
    height = atoi(argv[2]);
    minWidth = atoi(argv[3]);
    minHeight = atoi(argv[4]);
  }

  Dungeon d(width, height);
  BSPLayout<Dungeon::MapT> b(minWidth, minHeight);
  b.Build( d.GetMap() );

  std::cout << d;
  //  MonsterGenerator m;
  //  ItemGenerator i;
  //  Populate(d, b, m, i);

  return 0;
}
