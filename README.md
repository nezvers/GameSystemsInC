# TileMap system in C [Raylib rendering]

![](https://github.com/nezvers/TileMap/raw/master/Resource/Preview.gif)

Header only libraries with separate elements - tileset.h, tilemap.h.
To implement .c parts either create .c files for each of them with IMPLEMENTATION guard flags:    
```
#define NEZ_TILESET_IMPLEMENTATION
#include "../tileset.h"
```
Or in single file keeping in mind that tilemap.h includes the tilese.h:   
```
#define NEZ_TILEMAP_IMPLEMENTATION
#include "../tilemap.h"
#define NEZ_TILESET_IMPLEMENTATION
#include "../tileset.h"
```

Built to be as independent system as possible, but for rendering texture and feature (TileMap grid) I choose a Raylib framework.
If using different framework only need to change tileset.h function TileSetDraw and tilemap.h function TileMapDrawGrid.
For example code is used Raylib.

Future plan is to make also an autotiling header.
