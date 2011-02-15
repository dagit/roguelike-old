#ifndef DUNGEONGENERATOR_H
#define DUNGEONGENERATOR_H

template <typename DungeonT, 
          typename LayoutBuilderT, 
          typename ActorGeneratorT, 
          typename ObjectGeneratorT>
void
Populate(DungeonT & d, LayoutBuilderT & layoutBuilder, ActorGeneratorT & actorGenerator, ObjectGeneratorT & objectGenerator)
{
  layoutBuilder.Build( d.GetMap() );
  actorGenerator.PlaceActors( d.GetMap() );
  objectGenerator.PlaceObjects( d.GetMap() );
}

#endif
