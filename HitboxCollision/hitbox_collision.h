/*
    Many implementations are taken from Javidx9 / OneLoneCoder 
    https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/OneLoneCoder_PGE_Rectangles.cpp
*/

#ifndef HITBOX_COLLISIONS_H
#define HITBOX_COLLISIONS_H


#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "math.h"

#ifndef NEZRECT_F
#define NEZRECT_F
typedef struct {
    float x;    // origin x
    float y;    // origin y
    float w;    // width
    float h;    // height
} NezRect_f;
#endif // NEZRECT_F


#ifndef NEZVEC2_F
#define NEZVEC2_F
typedef struct {
    float x;
    float y;
} NezVec2_f;
#endif // NEZVEC2_F


//tile collision types
#ifndef NEZGRIDMAP
#define NEZGRIDMAP
typedef struct{
    float x;
    float y;
    int w;      // width
    int h;      // height
    int s;      // cell size (square cells)
    int *cell;  // array of tiles
}NezGridMap;
#endif // NEZGRIDMAP


#ifndef NEZHBAPI
    #ifdef NEZ_HITBOX_STATIC
        #define NEZHBAPI static
    #else
        #define NEZHBAPI extern
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

    // Check if two hitboxes collides
    NEZHBAPI bool
        HitboxCheckHitbox(NezRect_f *a, NezRect_f *b);
    // Get remaining move distance after collision check. Internally calls HitboxCheckHitbox()
    NEZHBAPI void
        HitboxMoveAndCollide(NezRect_f *a, NezVec2_f *velocity, NezRect_f *b, int count);
    NEZHBAPI NezRect_f
        HitboxExpand(NezRect_f *rect, NezVec2_f *size);
    NEZHBAPI void
        HitboxListFromGrid(NezRect_f *rect, NezGridMap *grid, NezRect_f **outList, int *outCount);
    NEZHBAPI void
        HitboxMoveAndCollideGrid(NezRect_f *a, NezVec2_f *velocity, NezGridMap *grid);
    
#ifdef __cplusplus
}
#endif
#endif //HITBOX_COLLISIONS_H

#ifdef HITBOX_COLLISIONS_IMPLEMENTATION
#undef HITBOX_COLLISIONS_IMPLEMENTATION

static inline void swapf(float *a, float *b){
   float t;
   t  = *b;
   *b = *a;
   *a = t;
}

bool HitboxCheckHitbox(NezRect_f *a, NezRect_f *b){
    return  a->x < b->x + b->w && a->x + a->w > b->x && a->y< b->y + b->h && a->y + a->h > b->y;
}


void HitboxMoveAndCollide(NezRect_f *a, NezVec2_f *velocity, NezRect_f *b, int count){
    int i;
    bool hitX = false;
    bool hitY = false;
    
    a->x += velocity->x;
    for (i = 0; i < count; i++){
        if (HitboxCheckHitbox(a, &b[i])) {
            if (velocity->x > 0.0f) {
                a->x = (b[i].x - a->w);
                hitX = true;
            }
            else if (velocity->x < 0.0f) {
                a->x = (b[i].x + b[i].w);
                hitX = true;
            }
        }
    }
    if(hitX){velocity->x = 0.0f;}
    
    a->y += velocity->y;
    for (i = 0; i < count; i++){
        if (HitboxCheckHitbox(a, &b[i])) {
            if (velocity->y > 0.0f) {
                a->y = (b[i].y - a->h);
                hitY = true;
            }
            else if (velocity->y < 0.0f) {
                a->y = (b[i].y + b[i].h);
                hitY = true;
            }
        }
    }
    if(hitY){velocity->y = 0.0f;}
    printf("y: %f\n", velocity->y);
}

NezRect_f HitboxExpand(NezRect_f *rect, NezVec2_f *size){
    return (NezRect_f){
        size->x > 0 ? rect->x : rect->x + size->x,
        size->y > 0 ? rect->y : rect->y + size->y,
        size->x > 0 ? rect->w + size->x : rect->w - size->x,
        size->y > 0 ? rect->h + size->y : rect->h - size->y
        };
}

void HitboxListFromGrid(NezRect_f *rect, NezGridMap *grid, NezRect_f **outList, int *outCount){
    float offX = rect->x - grid->x;
    float offY = rect->y - grid->y;
    // compensate flooring
    if (offX < 0.0f){offX -= grid->s;}
    if (offY < 0.0f){offY -= grid->s;}
    
    // grid coordinates
    int X = (int)(offX / grid->s);
    int sizeX = (int)(rect->w / grid->s) + 1;
    int Y = (int)(offY / grid->s);
    int sizeY = (int)(rect->h / grid->s) + 1;
    
    *outList = malloc(sizeof(NezRect_f) * sizeX * sizeY);
    //printf("offX: %d, %d\n", sizeX, sizeY);
    *outCount = 0;
    
    for (int y = Y; y < Y + sizeY; y++){
        if (y >= 0 && y < grid->h){
            for (int x = X; x < X + sizeX; x++){
                if (x >= 0 && x < grid->w){
                    int tile = grid->cell[x + y * grid->w];
                    if (tile){
                        (*outList)[*outCount] = (NezRect_f){
                            grid->x + x * grid->s,
                            grid->y + y * grid->s,
                            grid->s,
                            grid->s
                            };
                        *outCount += 1;
                    }
                }
            }
        }
    }
}

void HitboxMoveAndCollideGrid(NezRect_f *a, NezVec2_f *velocity, NezGridMap *grid){
    NezRect_f colArea = HitboxExpand(a, velocity);
    NezRect_f *tiles = (void*){0};
    int count;
    HitboxListFromGrid(&colArea, grid, &tiles, &count);
    HitboxMoveAndCollide(a, velocity, tiles, count);
    // free allocated RectList memory
    if(tiles){free(tiles);}
}

#endif //HITBOX_COLLISIONS_IMPLEMENTATION

