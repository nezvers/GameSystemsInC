# TileMap system in C [Raylib rendering]

`void TileMapDrawExWorld(TileMap *tileMap, int x, int y, int width, int height)`    
![Preview GIF](https://github.com/nezvers/GameSystemsInC/raw/master/TileMapSystem/Example/Resource/Preview.gif)    
`void AutoTileSetCell(AutoTile *autoTile, int x, int y)`    
![autotile](https://github.com/nezvers/GameSystemsInC/raw/master/TileMapSystem/Example/Resource/Preview_autotile.gif)     
`void TileMapSetTileResize(TileMap *tileMap, int x, int y, int id)`    
![resize](https://github.com/nezvers/GameSystemsInC/raw/master/TileMapSystem/Example/Resource/Preview_TileMap_resize.gif)    
`void TileMapDrawExSpreadWorld(TileMap *tileMap, int x, int y, int width, int height, float spreadX, float spreadY)`    
- influenced by "Door in the Woods". Requires game specific alterations for fog of war and height gradient    
![spread](https://github.com/nezvers/GameSystemsInC/raw/master/TileMapSystem/Example/Resource/Preview_TileMap_spread.gif)    
    
Header only libraries with separate elements - tileset.h, tilemap.h, autotile.h.
To implement .c parts need to `#define` IMPLEMENTATION guard flags before `#include` of the header file:    
```
#define NEZ_TILESET_IMPLEMENTATION
#include "tileset.h"
```


