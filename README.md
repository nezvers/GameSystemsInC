# TileMap system in C [Raylib rendering]

## Very early build and heavy work in progress. High probability of bugs.

`TileMapDrawExWorld(TileMap *tileMap, int x, int y, int width, int height)`    
![](https://github.com/nezvers/TileMap/raw/master/Resource/Preview.gif)    
`void AutoTileSetCell(AutoTile *autoTile, int x, int y)`    
![](https://github.com/nezvers/TileMap/raw/master/Resource/Preview_autotile.gif)     
`TileMapSetTileResize(TileMap *tileMap, int x, int y, int id)`    
![](https://github.com/nezvers/TileMap/raw/master/Resource/Preview_TileMap_resize.gif)    
    
Header only libraries with separate elements - tileset.h, tilemap.h, autotile.h.
To implement .c parts need to `#define` IMPLEMENTATION guard flags before `#include` of the header file:    
```
#define NEZ_TILESET_IMPLEMENTATION
#include "tileset.h"
```

Built to be as independent system as possible, but for rendering texture and drawing lines I used the Raylib framework.
If using different framework only changes need to be done:    
tileset.h    
- TileSetDrawTile()

tilemap.h
- TileMapDrawGrid()

Example code used Raylib.

