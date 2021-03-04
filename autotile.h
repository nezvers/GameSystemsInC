/*
#define NEZ_AUTOTILE_IMPLEMENTATION to include .c implementation
*/

#ifndef  NEZ_AUTOTILE_H
#define NEZ_AUTOTILE_H

#include "raylib.h"
#include <stdint.h>
#include "tilemap.h"

#define BMASK uint8_t

typedef struct {
    TileMap *tileMap;
    TileSet *tileSet;
    BMASK *bitmask;      // hold bitmask rules for each tile ID - 3x3 bitmask
    BMASK lookup[256];    // lookup table for tiles by bitmask - 9 bits
}AutoTile;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NEZATAPI
    #ifdef NEZ_AUTOTILE_STATIC
        #define NEZATAPI static
    #else
        #define NEZATAPI extern
    #endif
#endif

NEZATAPI AutoTile*
AutoTileNew(void);
NEZATAPI void
AutoTileDestroy(AutoTile *autoTile);
NEZATAPI AutoTile*
AutoTileNewInit(TileMap *tileMap);
NEZATAPI void
AutoTileInit(AutoTile *autoTile, TileMap *tileMap);
NEZATAPI void
AutoTileSetBitmask(AutoTile *autoTile, int *data, int dataSize);
NEZATAPI void
AutoTileSetBitmask(AutoTile *autoTile, int *data, int dataSize);
NEZATAPI void
AutoTileSetLookup(AutoTile *autoTile);
NEZATAPI int
AutoTileGetBitmask(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileSetCell(AutoTile *autoTile, int x, int y, int id);
NEZATAPI void
AutoTileUpdateCell(AutoTile *autoTile, int x, int y);

#ifdef __cplusplus
}
#endif
#endif //NEZ_AUTOTILE_H



// .c implementation / definition
#ifdef NEZ_AUTOTILE_IMPLEMENTATION
#undef NEZ_AUTOTILE_IMPLEMENTATION

AutoTile* AutoTileNew(void){
    AutoTile *autoTile = MemAlloc(sizeof(AutoTile));
    return autoTile;
}

void AutoTileDestroy(AutoTile *autoTile){
    MemFree(autoTile->bitmask);
    MemFree(autoTile);
}

// TileMap has to have assigned TileSet
AutoTile* AutoTileNewInit(TileMap *tileMap){
    AutoTile *autoTile = AutoTileNew();
    AutoTileInit(autoTile, tileMap);
    return autoTile;
}

void AutoTileInit(AutoTile *autoTile, TileMap *tileMap){
    autoTile->tileMap = tileMap;
    autoTile->tileSet = tileMap->tileSet;
    
    if (autoTile->bitmask){MemFree(autoTile->bitmask);}
    autoTile->bitmask = MemAlloc(sizeof(BMASK) * autoTile->tileSet->tileCount);
}

void AutoTileSetBitmask(AutoTile *autoTile, int *data, int dataSize){
    if (dataSize > autoTile->tileSet->tileCount){return;}
    int it = dataSize == autoTile->tileSet->tileCount ? dataSize : autoTile->tileSet->tileCount;
    for (int i =0; i < it; i++){
        autoTile->bitmask[i] = (BMASK)data[i];
    }
}

void AutoTileSetLookup(AutoTile *autoTile){
    for(int i = 0; i < autoTile->tileSet->tileCount; i++){
        int bitmask = autoTile->bitmask[i];
        if (bitmask > -1){
            autoTile->lookup[ bitmask ] = i;
        }
    }
}

// -1 if empty
int AutoTileGetBitmask(AutoTile *autoTile, int x, int y){
    int id = TileMapGetTile(autoTile->tileMap, x, y);
    if (id == -1){ return -1; }
    int bitmask = 0;
    
    // scan tiles around
    bitmask += (int)(TileMapGetTile(autoTile->tileMap, x-1, y-1) > -1);          // -------------
    bitmask += (int)(TileMapGetTile(autoTile->tileMap, x,   y-1) > -1) << 1;     // | 1  2  4   |
    bitmask += (int)(TileMapGetTile(autoTile->tileMap, x+1, y-1) > -1) << 2;     // | 8  0  16  |
    bitmask += (int)(TileMapGetTile(autoTile->tileMap, x-1, y)   > -1) << 3;     // | 32 64 128 |
    bitmask += (int)(TileMapGetTile(autoTile->tileMap, x+1, y)   > -1) << 4;     // -------------
    bitmask += (int)(TileMapGetTile(autoTile->tileMap, x-1, y+1) > -1) << 5;
    bitmask += (int)(TileMapGetTile(autoTile->tileMap, x,   y+1) > -1) << 6;
    bitmask += (int)(TileMapGetTile(autoTile->tileMap, x+1, y+1) > -1) << 7;
    
    return bitmask;
}

void AutoTileSetCell(AutoTile *autoTile, int x, int y, int id){
    TileMapSetTile(autoTile->tileMap, x, y, id);
    
    // update cells around
    AutoTileUpdateCell(autoTile, x-1, y-1);
    AutoTileUpdateCell(autoTile, x,   y-1);
    AutoTileUpdateCell(autoTile, x+1, y-1);
    AutoTileUpdateCell(autoTile, x-1, y);
    AutoTileUpdateCell(autoTile, x+1, y);
    AutoTileUpdateCell(autoTile, x-1, y+1);
    AutoTileUpdateCell(autoTile, x,   y+1);
    AutoTileUpdateCell(autoTile, x+1, y+1);
}

void AutoTileUpdateCell(AutoTile *autoTile, int x, int y){
    int bitmask = AutoTileGetBitmask(autoTile, x, y);
    if (bitmask == -1){
        TileMapSetTile(autoTile->tileMap, x, y, -1);
        return;
    }
    int id = autoTile->lookup[ bitmask ];
    TileMapSetTile(autoTile->tileMap, x, y, id);
}



#endif //NEZ_AUTOTILE_IMPLEMENTATION







