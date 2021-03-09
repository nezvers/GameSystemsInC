/*
#define NEZ_TILEMAP_IMPLEMENTATION to include .c implementation
*/

#ifndef  NEZ_TILEMAP_H
#define NEZ_TILEMAP_H


#include "raylib.h"
#include "tileset.h"
#include "stdio.h"

typedef struct{
    int* grid;
    TileSet * tileSet;
    RenderTexture2D texture;
    int x;
    int y;
    int width;
    int height;
}TileMap;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NEZTMAPI
    #ifdef NEZ_TILEMAP_STATIC
        #define NEZTMAPI static
    #else
        #define NEZTMAPI extern
    #endif
#endif

NEZTMAPI TileMap* 
TileMapNew();                                                                               // Allocates memory and give pointer to it
NEZTMAPI void 
TileMapDestroy(TileMap *tileMap);                                                           // Free allocated memory (TileMap and grid)    
NEZTMAPI void 
TileMapInitSize(TileMap *tileMap, int width, int height);                                   // Sets grid size and allocates memory for new size (for conserving grid data use TileMapResize)
NEZTMAPI void 
TileMapSetGridData(TileMap *tileMap, int *data, int dataSize);                              // Fill TileMap grid with array of data (if exceeds size the remain is not used)
NEZTMAPI int 
TileMapGetTile(TileMap *tileMap, int x, int y);                                             // Gets tile ID using tile coordinates
NEZTMAPI int 
TileMapGetTileWorld(TileMap *tileMap, int x, int y);                                        // Gets tile ID using world coordinates
NEZTMAPI void 
TileMapSetTile(TileMap *tileMap, int x, int y, int id);                                     // Sets ID of given grid slot
NEZTMAPI TilePosition 
TileMapSetTileResize(TileMap *tileMap, int x, int y, int id);                                     // Sets ID of given grid slot
NEZTMAPI void
TileMapResize(TileMap *tileMap, int left, int top, int right, int bottom);                  // Moves TileMap edges by given amount
NEZTMAPI TilePosition 
TileMapTrim(TileMap *tileMap);                                                              // Resizes TileMap if there is unused outer collumns or rows
NEZTMAPI TilePosition 
TileMapWorld2Tile(TileMap *tileMap, int x, int y);                                          // Convert world coordinates to tile coordinates
NEZTMAPI TilePosition 
TileMapTile2World(TileMap *tileMap, int x, int y);                                          // Convert tile coordinates to world coordinates
NEZTMAPI void 
TileMapClearGrid(TileMap *tileMap);                                                         // Remove all tile IDs (sets to -1)
NEZTMAPI void
TileMapClearGridEx(TileMap *tileMap, int x, int y, int w, int h);
NEZTMAPI void 
TileMapDraw(TileMap *tileMap);                                                              // Draw entire TileMap
NEZTMAPI void 
TileMapDrawEx(TileMap *tileMap, int x, int y, int width, int height);                       // Draw part of TileMap giving tile coordinates
NEZTMAPI void 
TileMapDrawExWorld(TileMap *tileMap, int x, int y, int width, int height);                  // Draw part of TileMap giving world coordinates (example: camera's rectangle)
NEZTMAPI void 
TileMapDrawPart(TileMap *tileMap, int posX, int posY, int x, int y, int width, int height); // Draw part of TileMap ignoring TIleMap position
NEZTMAPI void 
TileMapDrawGrid(TileMap *tileMap, Color color);

#ifdef __cplusplus
}
#endif
#endif //NEZ_TILEMAP_H

#ifdef NEZ_TILEMAP_IMPLEMENTATION
#undef NEZ_TILEMAP_IMPLEMENTATION
TileMap* TileMapNew(){
    TileMap *tileMap = malloc(sizeof(TileMap));
    *tileMap = (TileMap){0};
    TileMapInitSize(tileMap, 1, 1);
    return tileMap;
}

void TileMapDestroy(TileMap *tileMap){
    if(tileMap->grid){free(tileMap->grid);}
    free(tileMap);
}

void TileMapInitSize(TileMap *tileMap, int width, int height){
    tileMap->width = width;
    tileMap->height = height;
    if (tileMap->grid){free(tileMap->grid);}
    tileMap->grid = malloc(sizeof(int)*width*height);
    TileMapClearGrid(tileMap);
}

void TileMapSetGridData(TileMap *tileMap, int *data, int dataSize){
    int count = tileMap->width*tileMap->height > dataSize ? dataSize : tileMap->width*tileMap->height;
    for (int i = 0; i < count; i++){
        tileMap->grid[i] = data[i];
    }
}

// TileMap coordinates
int TileMapGetTile(TileMap *tileMap, int x, int y){
    bool lessX = x < 0;
    bool lessY = y < 0;
    bool biggerX = x > tileMap->width-1;
    bool biggerY = y > tileMap->height-1;
    if (lessX || lessY || biggerX || biggerY){
        return -1;
    }
    return tileMap->grid[tileMap->width*y + x];
}

// World coordinates
int TileMapGetTileWorld(TileMap *tileMap, int x, int y){
    bool lessX = x < tileMap->x;
    bool lessY = y < tileMap->x;
    bool biggerX = x > tileMap->x + (tileMap->width *tileMap->tileSet->tileX);
    bool biggerY = y > tileMap->y + (tileMap->height *tileMap->tileSet->tileX);
    if (lessX || lessY || biggerX || biggerY){
        return -1;
    }
    x = (x -tileMap->x) / tileMap->tileSet->tileX;
    y = (y -tileMap->y) / tileMap->tileSet->tileY;
    return tileMap->grid[tileMap->width*y + x];
}

void TileMapSetTile(TileMap *tileMap, int x, int y, int id){
    if (id < -1 || id > tileMap->tileSet->tileCount -1){
        return;
    }
    bool xIn = x > -1 && x < tileMap->width;
    bool yIn = y > -1 && y < tileMap->height;
    // sets tile within existing size
    if (xIn && yIn){
        int pos = x + y*tileMap->width;
        tileMap->grid[pos] = id;
    }
}

TilePosition TileMapSetTileResize(TileMap *tileMap, int x, int y, int id){
    TilePosition offset = {0};
    
    if (id < -1 || id > tileMap->tileSet->tileCount -1){
        return offset;
    }
    bool xIn = x > -1 && x < tileMap->width;
    bool yIn = y > -1 && y < tileMap->height;
    
    // sets tile within existing size
    if (xIn && yIn){
        int pos = x + y*tileMap->width;
        tileMap->grid[pos] = id;
        if (id > -1){return offset;}
    }
    //RESIZE
    else if (id != -1){
        int left, top, right, bottom;
        left = x < 0 ? x : 0;
        top = y < 0 ? y : 0;
        right = x > tileMap->width -1 ? x - (tileMap->width-1) : 0;
        bottom = y > tileMap->height -1 ? y - (tileMap->height-1) : 0;
        TileMapResize(tileMap, left, top, right, bottom);
        x -= left;
        y -= top;
        offset = (TilePosition){left, top};
        tileMap->grid[x + y*tileMap->width] = id;
    }
    TilePosition trimOffset = TileMapTrim(tileMap);
    offset.x += trimOffset.x;
    offset.y += trimOffset.y;
    return offset;
}

void TileMapResize(TileMap *tileMap, int left, int top, int right, int bottom){
    // TO DO: negative resize bug
    // printf("%d, %d\n", left, tileMap->width +right);
    // if (left >= tileMap->width +right){
        // left = tileMap->width +right -left;
        // right = left - tileMap->width +right;
    // }
    // if (top > tileMap->height +bottom){
        // top = tileMap->height +bottom -top;
        // bottom = top - tileMap->height +bottom;
    // }
    
    tileMap->x += left*tileMap->tileSet->tileX;     //reposition horizontally
    tileMap->y += top*tileMap->tileSet->tileY;      //reposition vertically
    int w = tileMap->width -left +right;          //new width
    int h = tileMap->height -top +bottom;         //new height
    int* tmp = tileMap->grid;                     //preparing for deleting old pointer
    
    tileMap->grid = malloc(sizeof(int) *w *h);
    for(int i =0; i< w*h; i++){
        tileMap->grid[i] = -1;
    }
    
    for (int y = 0; y < tileMap->height; y++){
        if (y-top >= 0 && y-top < h){
            for (int x = 0; x< tileMap->width; x++){
                if (x-left >= 0 &&  x-left < w){
                    tileMap->grid[w*(y-top) + x-left] = tmp[tileMap->width*y + x ];
                }
            }
        }
    }
    free(tmp);
    tileMap->width = w;
    tileMap->height = h;
}

TilePosition TileMapTrim(TileMap *tileMap){
    // init to furthest values
    
    
    int left = tileMap->width -1;
    int top = tileMap->height -1;
    int right = 0;
    int bottom = 0;
    // iterate through grid
    for (int y=0; y<tileMap->height; y++){
        for (int x=0; x < tileMap->width; x++){
            // detect used grid sides
            if (tileMap->grid[tileMap->width*y + x] > -1){
                if (x > right) { right = x; }
                if (y > bottom) { bottom = y; }
                if (x < left) { left = x; }
                if (y < top)  { top = y; }
            }
        }
    }
    right -= (tileMap->width-1);
    bottom -= (tileMap->height-1);
    TilePosition offset = {0};
    if (left == tileMap->width-1 && right == -(tileMap->width-1) && top == tileMap->height-1 && bottom == -(tileMap->height-1)){
        return offset;
    }
    if (left!=0 || top!=0 || right!=0 || bottom!=0){
        TileMapResize(tileMap, left, top, right, bottom);
    }
    return (TilePosition){left, top};
}

TilePosition TileMapWorld2Tile(TileMap *tileMap, int x, int y){
    x = (x-tileMap->x);
    y = (y-tileMap->y);
    if(x < 0){x -= tileMap->tileSet->tileX;}
    if(y < 0){y -= tileMap->tileSet->tileY;}
    x = x/tileMap->tileSet->tileX;
    y = y/tileMap->tileSet->tileY;
    return (TilePosition){x, y};
}

TilePosition TileMapTile2World(TileMap *tileMap, int x, int y){
    int _x = x * tileMap->tileSet->tileX + tileMap->x;
    int _y = y * tileMap->tileSet->tileY + tileMap->y;
    return (TilePosition){_x, _y};
}

void TileMapClearGrid(TileMap *tileMap){
    for(int i =0; i< tileMap->width*tileMap->height; i++){
        tileMap->grid[i] = -1;
    }
}

void TileMapClearGridEx(TileMap *tileMap, int x, int y, int w, int h){
    
}

void TileMapDraw(TileMap *tileMap){
    int w = tileMap->width;
    int h = tileMap->height;
    int px = tileMap->x;
    int py = tileMap->y;
    int sx = tileMap->tileSet->tileX;
    int sy = tileMap->tileSet->tileY;
    for (int y = 0; y < h; y++){
        for (int x = 0; x < w; x++){
            int id = tileMap->grid[x+y*w];
            if (id > -1){
                TileSetDrawTile(tileMap->tileSet, id, px + x*sx, py + y*sy);
            }
        }
    }
}

void TileMapDrawGrid(TileMap *tileMap, Color color){
    int x = tileMap->x;
    int y = tileMap->y;
    int w = tileMap->width;
    int h = tileMap->height;
    int sx = tileMap->tileSet->tileX;
    int sy = tileMap->tileSet->tileY;
    int lenX = w*sx;
    int lenY = h*sy;
    // horizontal lines
    for (int i = 0; i < h+1; i++){
        DrawLine(x, y + sy*i, x+lenX, y + sy*i, color);
    }
    // vertical lines
    for (int i = 0; i < w+1; i++){
        DrawLine(x + sx*i, y, x + sx*i, y+lenY, color);
    }
}

// Draw only part of TileMap in their original position.
// Tile coordinates. x & y must be left top corner.
void TileMapDrawEx(TileMap *tileMap, int x, int y, int width, int height){
    TileMapDrawPart(tileMap, tileMap->x, tileMap->y, x, y, width, height);
}

// World coordinates. x & y must be left top corner.
void TileMapDrawExWorld(TileMap *tileMap, int x, int y, int width, int height){
    // find tile coordinates
    int distX = x - tileMap->x;
    int distY = y - tileMap->y;
    int signX = distX >= 0 ? 1 : -1;
    int signY = distY >= 0 ? 1 : -1;
    int tileSizeX = tileMap->tileSet->tileX;
    int tileSizeY = tileMap->tileSet->tileY;
    int X = distX / tileSizeX;
    int Y = distY / tileSizeY;
    // compensate flooring if negative cell positions
    if (signX < 0 && X*tileSizeX != distX){
        X += signX;
    }
    if (signY < 0 && Y*tileSizeY != distY){
        Y += signY;
    }
    int W = (width / tileSizeX) +1;
    int H = (height / tileSizeY) +1;
    TileMapDrawPart(tileMap, tileMap->x, tileMap->y, X, Y, W, H);
}

// Can be useful to render into a RenderTexture for a single texture draw if view doesn't changes.
void TileMapDrawPart(TileMap *tileMap, int posX, int posY, int x, int y, int width, int height){
    if (x > tileMap->width || x+width < 0 || y > tileMap->height || y+height < 0){
        return;
    }
    // find used cells
    int w = x+width < tileMap->width ? x+width : tileMap->width;
    int h = y+height < tileMap->height ? y+height : tileMap->height;
    x = x > 0 ? x : 0;
    y = y > 0 ? y : 0;
    int px = posX;
    int py = posY;
    int sx = tileMap->tileSet->tileX;
    int sy = tileMap->tileSet->tileY;
    int gridW = tileMap->width;
    for (int _y = y; _y < h; _y++){
        for (int _x = x; _x < w; _x++){
            TileSetDrawTile(tileMap->tileSet, tileMap->grid[_x+_y*gridW], px + _x*sx, py + _y*sy);
        }
    }
}

#endif //NEZ_TILEMAP_IMPLEMENTATION











