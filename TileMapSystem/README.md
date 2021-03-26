# TileMap system in C [Raylib rendering]

## Very early build and heavy work in progress. High probability of bugs.

`TileMapDrawExWorld(TileMap *tileMap, int x, int y, int width, int height)`    
![](https://github.com/nezvers/GameSystemsInC/raw/master/TileMapSystem/Resource/Preview.gif)    
`void AutoTileSetCell(AutoTile *autoTile, int x, int y)`    
![](https://github.com/nezvers/GameSystemsInC/raw/master/TileMapSystem/Resource/Preview_autotile.gif)     
`TileMapSetTileResize(TileMap *tileMap, int x, int y, int id)`    
![](https://github.com/nezvers/GameSystemsInC/raw/master/TileMapSystem/Resource/Preview_TileMap_resize.gif)    
`void TileMapDrawExSpreadWorld(TileMap *tileMap, int x, int y, int width, int height, float spreadX, float spreadY)`    
- influenced by "Door in the Woods". Requires game specific alterations for fog of war and height gradient    
![](https://github.com/nezvers/GameSystemsInC/raw/master/TileMapSystem/Resource/Preview_TileMap_spread.gif)    
    
Header only libraries with separate elements - tileset.h, tilemap.h, autotile.h.
To implement .c parts need to `#define` IMPLEMENTATION guard flags before `#include` of the header file:    
```
#define NEZ_TILESET_IMPLEMENTATION
#include "tileset.h"
```


