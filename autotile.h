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
// TileMap has to have assigned TileSet
NEZATAPI AutoTile*
AutoTileNewInitTileRegion(TileMap *tileMap, int tx, int ty, int tw, int th);        // create new tilemap, requires region of TileSet
NEZATAPI AutoTile*
AutoTileNewInitTileList(TileMap *tileMap, int *tileList, int tileCount);            // create new tilemap, requires array of tile IDs
NEZATAPI void
AutoTileInit(AutoTile *autoTile, TileMap *tileMap, int *tileList, int tileCount);   // Init existing autotile, requires region of TileSet
NEZATAPI void
AutoTileSetBitmaskData(AutoTile *autoTile, int *data, int dataSize);
NEZATAPI int
AutoTileGetBitmask(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileSetCell(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileSetCellResize(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileSetCellResizeWorld(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileRemoveCell(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileRemoveCellResize(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileRemoveCellResizeWorld(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileUpdateCell(AutoTile *autoTile, int x, int y);
NEZATAPI void
AutoTileUpdateCellsAround(AutoTile *autoTile, int x, int y);
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


AutoTile* AutoTileNewInitTileRegion(TileMap *tileMap, int tx, int ty, int tw, int th){
    AutoTile *autoTile = AutoTileNew();
    
    // create tileList
    int tileCount = tw*th;
    int *tileList = malloc(sizeof(int) *tileCount);
    int col = tileMap->tileSet->collumns;
    for (int y=0; y<th; y++){
        for (int x=0; x < tw; x++){
            int id = tx+ty*col + x+y*col;
            tileList[x+y*tw] = id;
        }
    }
    AutoTileInit(autoTile, tileMap, tileList, tileCount);
    free(tileList);
    return autoTile;
}

AutoTile* AutoTileNewInitTileList(TileMap *tileMap, int *tileList, int tileCount){
    AutoTile *autoTile = AutoTileNew();
    
    AutoTileInit(autoTile, tileMap, tileList, tileCount);
    free(tileList);
    return autoTile;
}

void AutoTileInit(AutoTile *autoTile, TileMap *tileMap, int *tileList, int tileCount){
    autoTile->tileMap = tileMap;
    autoTile->tileSet = tileMap->tileSet;
    
    if (autoTile->id){free(autoTile->id);}
    autoTile->id = malloc(sizeof(int) *tileCount);
    
    // assign tileSet IDs to autoTile->id
    for (int i=0; i<tileCount; i++){
        autoTile->id[i] = tileList[i];
    }
}

// dataSize must not exceed tileList
void AutoTileSetBitmaskData(AutoTile *autoTile, int *data, int dataSize){
    // find tile ID with most bits in each position
    for(int i = 0; i < 256; i++){
        int maxBits = -1;
        int id = 0;
        // flip all bits in i
        int negativeMask = i ^ 0xFF;
        for(int j = 0; j < dataSize; j++){
            int bitmask = data[j];
            
            // accept only if doesn't have bits that "i" doesn't have
            if (bitmask > -1 && !(negativeMask & bitmask)){
                int bits = GetSetBitCount(bitmask & i);
                if (bits > maxBits){
                    maxBits = bits;
                    id = j;
                }
            }
        }
        autoTile->lookup[ i ] = id;
    }
}

// -1 if empty
int AutoTileGetBitmask(AutoTile *autoTile, int x, int y){
    int bitmask = 0;
    
    // Neighbour values
    // -------------
    // | 1  2  4   |
    // | 8  0  16  |
    // | 32 64 128 |
    // -------------
    // scan tiles around and set coresponding bits
    bitmask += (int)(TileMapGetTile(autoTile->tileMap, x-1, y-1) > -1);
    bitmask += (int)(TileMapGetTile(autoTile->tileMap, x,   y-1) > -1) << 1;
    bitmask += (int)(TileMapGetTile(autoTile->tileMap, x+1, y-1) > -1) << 2;
    bitmask += (int)(TileMapGetTile(autoTile->tileMap, x-1, y)   > -1) << 3;
    bitmask += (int)(TileMapGetTile(autoTile->tileMap, x+1, y)   > -1) << 4;
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
    
    AutoTileUpdateCellsAround(autoTile, x, y);
}


void AutoTileSetCellResize(AutoTile *autoTile, int x, int y){
    int bitmask = AutoTileGetBitmask(autoTile, x, y);
    // if empty cell, then new tile has bitmask of 0
    bitmask = bitmask > -1 ? bitmask : 0;
    int id = autoTile->id[ autoTile->lookup[bitmask] ];
    TilePosition offset = TileMapSetTileResize(autoTile->tileMap, x, y, id);
    
    
    AutoTileUpdateCellsAround(autoTile, x -offset.x, y -offset.y);
}


void AutoTileSetCellResizeWorld(AutoTile *autoTile, int x, int y){
    TilePosition tp = TileMapWorld2Tile(autoTile->tileMap, x, y);
    AutoTileSetCellResize(autoTile, tp.x, tp.y);
}

void AutoTileRemoveCell(AutoTile *autoTile, int x, int y){
    TileMapSetTile(autoTile->tileMap, x, y, -1);
    
    AutoTileUpdateCellsAround(autoTile, x, y);
}

void AutoTileRemoveCellResize(AutoTile *autoTile, int x, int y){
    TilePosition offset = TileMapSetTileResize(autoTile->tileMap, x, y, -1);
    
    AutoTileUpdateCellsAround(autoTile, x -offset.x, y -offset.y);
}

void AutoTileRemoveCellResizeWorld(AutoTile *autoTile, int x, int y){
    TilePosition tp = TileMapWorld2Tile(autoTile->tileMap, x, y);
    AutoTileRemoveCellResize(autoTile, tp.x, tp.y);
}

void AutoTileUpdateCell(AutoTile *autoTile, int x, int y){
    int id = TileMapGetTile(autoTile->tileMap, x, y);
    if (id < 0){return;}
    
    int bitmask = AutoTileGetBitmask(autoTile, x, y);
    
    //reusing same variable
    id = autoTile->id[ autoTile->lookup[bitmask] ];
    TileMapSetTile(autoTile->tileMap, x, y, id);
}

void AutoTileUpdateCellsAround(AutoTile *autoTile, int x, int y){
    AutoTileUpdateCell(autoTile, x-1, y-1);
    AutoTileUpdateCell(autoTile, x,   y-1);
    AutoTileUpdateCell(autoTile, x+1, y-1);
    AutoTileUpdateCell(autoTile, x-1, y);
    AutoTileUpdateCell(autoTile, x+1, y);
    AutoTileUpdateCell(autoTile, x-1, y+1);
    AutoTileUpdateCell(autoTile, x,   y+1);
    AutoTileUpdateCell(autoTile, x+1, y+1);
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


