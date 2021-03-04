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
AutoTileSetBitmaskData(AutoTile *autoTile, int *data, int dataSize);
NEZATAPI void
AutoTileSetLookup(AutoTile *autoTile);
NEZATAPI int
AutoTileGetBitmask(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileSetCell(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileUpdateCell(AutoTile *autoTile, int x, int y);
NEZATAPI int
GetSetBitCount(int n);

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

void AutoTileSetBitmaskData(AutoTile *autoTile, int *data, int dataSize){
    if (dataSize > autoTile->tileSet->tileCount){return;}
    int it = dataSize == autoTile->tileSet->tileCount ? dataSize : autoTile->tileSet->tileCount;
    for (int i =0; i < it; i++){
        autoTile->bitmask[i] = (BMASK)data[i];
    }
    AutoTileSetLookup(autoTile);
}

void AutoTileSetLookup(AutoTile *autoTile){
    int tileCount = autoTile->tileSet->tileCount;
    
    // find tile ID with most bits in each position
    for(int i = 1; i < 256; i++){
        int maxBits = 0;
        int mask = 0;
        int negativeMask = i ^ 0xFF;
        for(int j = 1; j < tileCount; j++){
            int bitmask = autoTile->bitmask[i];
            
            // accept only if doesn't have bits that "i" doesn't have
            if (bitmask > -1 && !(negativeMask & bitmask)){
                int bits = GetSetBitCount(bitmask & i);
                if (bits > maxBits){
                    maxBits = bits;
                    mask = bitmask;
                }
            }
        }
        autoTile->lookup[ i ] = mask;
    }
    
    // find the center tile ID with bitmask of 0
    for(int j = 1; j < tileCount; j++){
        int bitmask = autoTile->bitmask[j];
        if (bitmask > -1){
            int bits = GetSetBitCount(bitmask);
            if (bits == 0){
                autoTile->lookup[ 0 ] = j;
            }
        }
    }
}

// -1 if empty
int AutoTileGetBitmask(AutoTile *autoTile, int x, int y){
    int id = TileMapGetTile(autoTile->tileMap, x, y);
    if (id == -1){ return -1; }
    int bitmask = 0;
    
    // scan tiles around and set coresponding bits
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

void AutoTileSetCell(AutoTile *autoTile, int x, int y){
    int bitmask = AutoTileGetBitmask(autoTile, x, y);
    bitmask = bitmask > -1 ? bitmask : 0;
    int id = autoTile->lookup[bitmask];
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
        return;
    }
    int id = autoTile->lookup[ bitmask ];
    TileMapSetTile(autoTile->tileMap, x, y, id);
}

int GetSetBitCount(int n){
    int count = 0;
    while (n) {
        n &= (n - 1);
        count++;
    }
    return count;
}

#endif //NEZ_AUTOTILE_IMPLEMENTATION



