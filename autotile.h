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
    int *id;      // hold bitmask rules for each tile ID - 3x3 bitmask
    int tileCount;
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
AutoTileNewInit(TileMap *tileMap, int tx, int ty, int tw, int th);                  // create new tilemap, requires region of TileSet
NEZATAPI void
AutoTileInit(AutoTile *autoTile, TileMap *tileMap, int tx, int ty, int tw, int th); // Init existing autotile, requires region of TileSet
NEZATAPI void
AutoTileSetBitmaskData(AutoTile *autoTile, int *data, int dataSize);
NEZATAPI void
AutoTileSetLookup(AutoTile *autoTile);
NEZATAPI int
AutoTileGetBitmask(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileSetCell(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileSetCellResize(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileRemoveCell(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileRemoveCellResize(AutoTile *autoTile, int x, int y);
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
    AutoTile *autoTile = malloc(sizeof(AutoTile));
    *autoTile = (AutoTile){0}; // set all to 0
    return autoTile;
}

void AutoTileDestroy(AutoTile *autoTile){
    if (autoTile->id){free(autoTile->id);}
    free(autoTile);
}

// TileMap has to have assigned TileSet
AutoTile* AutoTileNewInit(TileMap *tileMap, int tx, int ty, int tw, int th){
    AutoTile *autoTile = AutoTileNew();
    AutoTileInit(autoTile, tileMap, tx, ty, tw, th);
    return autoTile;
}

void AutoTileInit(AutoTile *autoTile, TileMap *tileMap, int tx, int ty, int tw, int th){
    autoTile->tileMap = tileMap;
    autoTile->tileSet = tileMap->tileSet;
    
    if (autoTile->id){free(autoTile->id);}
    autoTile->id = malloc(sizeof(int) * tw *th);
    autoTile->tileCount = tw*th;
    
    // assign tileSet IDs to autoTile->id
    int col = autoTile->tileSet->collumns;
    for (int y=0; y<th; y++){
        for (int x=0; x < tw; x++){
            int id = tx+ty*col + x+y*col;
            autoTile->id[x+y*tw] = id;
        }
    }
}

void AutoTileSetBitmaskData(AutoTile *autoTile, int *data, int dataSize){
    int it = dataSize < autoTile->tileCount ? dataSize : autoTile->tileCount;
    
    // find tile ID with most bits in each position
    for(int i = 0; i < 256; i++){
        int maxBits = -1;
        int id = 0;
        int negativeMask = i ^ 0xFF;
        for(int j = 0; j < it; j++){
            int bitmask = data[j];
            
            // accept only if doesn't have bits that "i" doesn't have
            if (bitmask > -1 && !(negativeMask & bitmask)){
                int bits = GetSetBitCount(bitmask & i);
                if (bits > maxBits){
                    maxBits = bits;
                    id = autoTile->id[j];
                }
            }
        }
        autoTile->lookup[ i ] = id;
    }
}

// -1 if empty
int AutoTileGetBitmask(AutoTile *autoTile, int x, int y){
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
    
    if (bitmask == 0){
        int id = TileMapGetTile(autoTile->tileMap, x, y);
        if (id == -1){ return -1; }
    }
    return bitmask;
}

void AutoTileSetCell(AutoTile *autoTile, int x, int y){
    int bitmask = AutoTileGetBitmask(autoTile, x, y);
    bitmask = bitmask > -1 ? bitmask : 0;
    int id = autoTile->id[ autoTile->lookup[bitmask] ];
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

void AutoTileSetCellResize(AutoTile *autoTile, int x, int y){
    int bitmask = AutoTileGetBitmask(autoTile, x, y);
    bitmask = bitmask > -1 ? bitmask : 0;
    int id = autoTile->id[ autoTile->lookup[bitmask] ];
    TileMapSetTileResize(autoTile->tileMap, x, y, id);
    
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

void AutoTileRemoveCell(AutoTile *autoTile, int x, int y){
    TileMapSetTile(autoTile->tileMap, x, y, -1);
    
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

void AutoTileRemoveCellResize(AutoTile *autoTile, int x, int y){
    TileMapSetTileResize(autoTile->tileMap, x, y, -1);
    
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
    int id = TileMapGetTile(autoTile->tileMap, x, y);
    if (id < 0){return;}
    
    int bitmask = AutoTileGetBitmask(autoTile, x, y);
    
    id = autoTile->lookup[ bitmask ];
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


