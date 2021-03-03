# TileMap system in C [Raylib rendering]

![](https://github.com/nezvers/TileMap/raw/master/Resource/Preview.gif)

Header only libraries with separate elements - tileset.h, tilemap.h.
To implement .c parts need to `#define` IMPLEMENTATION guard flags before `#include` of the header file:    
```
#define NEZ_TILESET_IMPLEMENTATION
#include "../tileset.h"
```

Built to be as independent system as possible, but for rendering texture and memory allocation I used the Raylib framework.
If using different framework only need to change:    
tileset.h    
- TileSetDrawTile()
- MemAlloc()
- MemFree()

tilemap.h
- TileMapDrawGrid()
- MemAlloc()
- MemFree()

Example code used Raylib.

Future plan is to make also an autotiling header.
