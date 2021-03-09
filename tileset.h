/*
#define NEZ_TILESET_IMPLEMENTATION to include .c implementation
*/

#ifndef  NEZ_TILESET_H
#define NEZ_TILESET_H

#include "raylib.h"
#include "stdlib.h"


//Used for custom positions and TileMap positions
typedef struct{
    int x;
    int y;
}TilePosition;

typedef struct{
    Texture2D texture;
    int tileX;
    int tileY;
    int collumns;
    int rows;
    int tileCount;
    bool customPositions;   // used in TileSetDrawTile
    TilePosition *positionList;
}TileSet;


#ifdef __cplusplus
extern "C" {
#endif

#ifndef NEZTSAPI
    #ifdef NEZ_TILESET_STATIC
        #define NEZTSAPI static
    #else
        #define NEZTSAPI extern
    #endif
#endif

NEZTSAPI TileSet*
TileSetNew();                                                                                                                   // Allocates memory and give pointer to it

// if have custom positions on texture, give array of TilePositions otherwise NULL and positionCount = 0 or less
NEZTSAPI TileSet*
TileSetNewInitFromFile(const char * fileName, int tileWidth, int tileHeight, TilePosition *positionList, int positionCount);    // Allocates memory, gives pointer to it and initializes sizes
NEZTSAPI TileSet*
TileSetNewInitFromMemory(Texture texture, int tileWidth, int tileHeight, TilePosition *positionList, int positionCount);        // Allocates memory, gives pointer to it and initializes sizes
NEZTSAPI void
TileSetDestroy(TileSet* tileSet);                                                                                               // Free allocated memory
NEZTSAPI void
TileSetDestroyWithTexture(TileSet* tileSet);                                                                                    // Free allocated memory and unloads texture
NEZTSAPI void
TileSetSetPositionList(TileSet *tileSet, TilePosition *positionList, int positionCount);                                        // Sets list of custom positions that are used to draw tiles by ID
NEZTSAPI void
TileSetSetSize(TileSet *tileSet, int tileWidth, int tileHeight);                                                                // Set tile size and tile IDs (Texture must be assigned)
NEZTSAPI void
TileSetDrawTile(TileSet *tileSet, int id, int x, int y);                                                                        // Draws tile at given position

#ifdef __cplusplus
}
#endif
#endif //NEZ_TILESET_H

#ifdef NEZ_TILESET_IMPLEMENTATION
#undef NEZ_TILESET_IMPLEMENTATION

TileSet*    TileSetNew(){
    TileSet *tileSet = malloc(sizeof(TileSet));
    *tileSet = (TileSet){0};
    return tileSet;
}

TileSet* TileSetNewInitFromFile(const char * fileName, int tileWidth, int tileHeight, TilePosition *positionList, int positionCount){
    TileSet *tileSet = TileSetNew();
    tileSet->texture = LoadTexture(fileName);
    if (positionCount < 1){
        tileSet->customPositions = false;
        TileSetSetSize(tileSet, tileWidth, tileHeight);
    }
    else{
        tileSet->tileX = tileWidth;
        tileSet->tileY = tileHeight;
        TileSetSetPositionList(tileSet, positionList, positionCount);
    }
    return tileSet;
}

TileSet* TileSetNewInitFromMemory(Texture texture, int tileWidth, int tileHeight, TilePosition *positionList, int positionCount){
    TileSet *tileSet = TileSetNew();
    tileSet->texture = texture;
    if (positionCount < 1){
        tileSet->customPositions = false;
        TileSetSetSize(tileSet, tileWidth, tileHeight);
    }
    else{
        tileSet->tileX = tileWidth;
        tileSet->tileY = tileHeight;
        TileSetSetPositionList(tileSet, positionList, positionCount);
    }
    return tileSet;
}

void TileSetDestroy(TileSet* tileSet){
    if(tileSet->positionList){free(tileSet->positionList);}
    free(tileSet);
}

void TileSetDestroyWithTexture(TileSet* tileSet){
    UnloadTexture(tileSet->texture);
    TileSetDestroy(tileSet);
}

void TileSetSetPositionList(TileSet *tileSet, TilePosition *positionList, int positionCount){
    tileSet->customPositions = true;
    tileSet->positionList = (TilePosition*)malloc(sizeof(TilePosition) * positionCount);
    for(int i=0; i<positionCount; i++){
        tileSet->positionList[i] = positionList[i];
    }
}

// texture must be assigned
void TileSetSetSize(TileSet *tileSet, int tileWidth, int tileHeight){
    tileSet->tileX = tileWidth;
    tileSet->tileY = tileHeight;
    tileSet->collumns = tileSet->texture.width/tileWidth;
    tileSet->rows = tileSet->texture.height/tileHeight;
    tileSet->tileCount = tileSet->collumns * tileSet->rows;
}

void TileSetDrawTile(TileSet *tileSet, int id, int x, int y){
    if(!tileSet->customPositions){
        int col = id % tileSet->collumns;
        int row = id / tileSet->collumns;
        Rectangle tileRect = {(float)col * tileSet->tileX, (float)row * tileSet->tileY, (float)tileSet->tileX, (float)tileSet->tileY};
        DrawTextureRec(tileSet->texture, tileRect, (Vector2){(float)x, (float)y}, WHITE);
    }
    else{
        Rectangle tileRect = {(float)tileSet->positionList[id].x, (float)tileSet->positionList[id].y, (float)tileSet->tileX, (float)tileSet->tileY};
        DrawTextureRec(tileSet->texture, tileRect, (Vector2){(float)x, (float)y}, WHITE);
    }
}

#endif //NEZ_TILESET_IMPLEMENTATION


