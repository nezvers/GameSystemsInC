/*
#define NEZ_TILESET_IMPLEMENTATION to include .c implementation
*/

#ifndef  NEZ_TILESET_H
#define NEZ_TILESET_H

#include "raylib.h"
#include "stdlib.h"

#ifndef NEZVEC2_I
#define NEZVEC2_I
//Used for custom positions and TileMap positions
typedef struct{
    int x;
    int y;
}NezVec2_i;
#endif // NEZVEC2_I

typedef struct{
    Texture2D texture;
    int tileX;
    int tileY;
    int collumns;
    int rows;
    int tileCount;
    bool customPositions;   // used in TileSetDrawTile
    NezVec2_i *positionList;
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
TileSetNew();                                                                                                               // Allocates memory and give pointer to it

// if have custom positions on texture, give array of TilePositions otherwise NULL and positionCount = 0 or less
NEZTSAPI TileSet*
TileSetNewInitFromFile(const char * fileName, int tileWidth, int tileHeight, NezVec2_i *positionList, int positionCount);// Allocates memory, gives pointer to it and initializes sizes
NEZTSAPI TileSet*
TileSetNewInitFromMemory(Texture texture, int tileWidth, int tileHeight, NezVec2_i *positionList, int positionCount);    // Allocates memory, gives pointer to it and initializes sizes
NEZTSAPI void
TileSetDestroy(TileSet* tileSet);                                                                                           // Free allocated memory
NEZTSAPI void
TileSetDestroyWithTexture(TileSet* tileSet);                                                                                // Free allocated memory and unloads texture
NEZTSAPI void
TileSetSetPositionList(TileSet *tileSet, NezVec2_i *positionList, int positionCount);                                    // Sets list of custom positions that are used to draw tiles by ID
NEZTSAPI void
TileSetSetSize(TileSet *tileSet, int tileWidth, int tileHeight);                                                            // Set tile size and tile IDs (Texture must be assigned)
NEZTSAPI void
TileSetDrawTile(TileSet *tileSet, int id, int x, int y);                                                                    // Draws tile at given position
NEZTSAPI void
TileSetDrawTileStandalone(Texture texture, int id, int x, int y, int tileCollumns, int tileWidth, int tileHeight);          // Draws tile without creating TileSet
NEZTSAPI void
TileSetDrawTileFromTexture(Texture texture, int x, int y, NezVec2_i texturePosition, int tileWidth, int tileHeight);     // Draws tile without creating TileSet, provide position on texture and tile size


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

TileSet* TileSetNewInitFromFile(const char * fileName, int tileWidth, int tileHeight, NezVec2_i *positionList, int positionCount){
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

TileSet* TileSetNewInitFromMemory(Texture texture, int tileWidth, int tileHeight, NezVec2_i *positionList, int positionCount){
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

void TileSetSetPositionList(TileSet *tileSet, NezVec2_i *positionList, int positionCount){
    tileSet->customPositions = true;
    tileSet->positionList = (NezVec2_i*)malloc(sizeof(NezVec2_i) * positionCount);
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
    if(!tileSet->customPositions && tileSet->collumns > 0){
        int col = id % tileSet->collumns;
        int row = id / tileSet->collumns;
        Rectangle tileRect = {(float)col * tileSet->tileX, (float)row * tileSet->tileY, (float)tileSet->tileX, (float)tileSet->tileY};
        DrawTextureRec(tileSet->texture, tileRect, (Vector2){(float)x, (float)y}, WHITE);
    }
    else if (tileSet->collumns > 0){
        Rectangle tileRect = {(float)tileSet->positionList[id].x, (float)tileSet->positionList[id].y, (float)tileSet->tileX, (float)tileSet->tileY};
        DrawTextureRec(tileSet->texture, tileRect, (Vector2){(float)x, (float)y}, WHITE);
    }
}

void TileSetDrawTileStandalone(Texture texture, int id, int x, int y, int tileCollumns, int tileWidth, int tileHeight){
	int col = id % tileCollumns;
	int row = id / tileCollumns;
	Rectangle tileRect = {(float)col * tileWidth, (float)row * tileHeight, (float)tileWidth, (float)tileHeight};
	DrawTextureRec(texture, tileRect, (Vector2){(float)x, (float)y}, WHITE);
}

void TileSetDrawTileFromTexture(Texture texture, int x, int y, NezVec2_i texturePosition, int tileWidth, int tileHeight){
	Rectangle tileRect = {(float)texturePosition.x, (float)texturePosition.y, (float)tileWidth, (float)tileHeight};
	DrawTextureRec(texture, tileRect, (Vector2){(float)x, (float)y}, WHITE);
}

#endif //NEZ_TILESET_IMPLEMENTATION

