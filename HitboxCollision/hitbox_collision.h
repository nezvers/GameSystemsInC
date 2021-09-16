/*
    Many implementations are taken from Javidx9 / OneLoneCoder 
    https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/OneLoneCoder_PGE_Rectangles.cpp
*/

#ifndef HITBOX_COLLISIONS_H
#define HITBOX_COLLISIONS_H


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
    char *cell;  // array of tiles
}NezGridMap;
#endif // NEZGRIDMAP

typedef struct {
    int size;
    NezRect_f *rect;
}RectList;

// Collision flags
enum {
    RIGHT_COL,
    LEFT_COL,
    TOP_COL,
    BOTTOM_COL,
    X_COL,
    Y_COL,
};

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

    NEZHBAPI char
        HitboxMoveAndCollide(NezRect_f *rect, NezVec2_f *velocity, NezRect_f *bodies, int count, NezGridMap *grid);
    // Check if two hitboxes collides
    NEZHBAPI bool
        HitboxCheckHitbox(NezRect_f *a, NezRect_f *b);
    NEZHBAPI char
        HitboxCollisionResolve(NezRect_f *rect, NezVec2_f *velocity, RectList *list);
    NEZHBAPI NezRect_f
        HitboxExpand(NezRect_f *rect, NezVec2_f *size);
    // Get grid cell area in grid coordinates. areaOut is int[4]
    NEZHBAPI void
        HitboxGridArea(NezRect_f *rect, NezGridMap *grid, int *areaOut);
    NEZHBAPI void
        HitboxListFromGrid(NezGridMap *grid, int *area, NezRect_f **outList, int *outCount);
        
    // FUTURE EXPANSION
    // Raycast against rectangle
    NEZHBAPI bool
        HitboxVsRaycast(NezVec2_f *rayOrig, NezVec2_f *rayDir, NezRect_f *target, NezVec2_f *colPoint, NezVec2_f *colNormal, float *timeStep);
    // Dynamic rectangle vs rectangle collision. Velocity gets multiplied by deltaTime. Pass deltaTime value of 1.0f if full velocity length should be performed.
    NEZHBAPI bool
        HitboxDynamicVsHitbox(NezRect_f *movingRect, NezVec2_f *velocity, NezRect_f *target, NezVec2_f *colPoint, NezVec2_f *colNormal, float *timeStep);
    // Resolve movement collision between rectangles
    NEZHBAPI bool
        HitboxResolveDynamicRectVsRect(NezRect_f *movingRect, NezVec2_f *velocity, NezRect_f *target);
    
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

bool HitboxCheckHitbox(NezRect_f *a, NezRect_f *b) {
    return  a->x < b->x + b->w && a->x + a->w > b->x && a->y< b->y + b->h && a->y + a->h > b->y;
}


char HitboxMoveAndCollide(NezRect_f *rect, NezVec2_f *velocity, NezRect_f *bodies, int count, NezGridMap *grid){
    NezRect_f colArea = HitboxExpand(rect, velocity);
    RectList colList = {0};
    int tileCount = 0;
    if (grid){
        int cellArea[4];
        HitboxGridArea(&colArea, grid, cellArea);
        colList.rect = malloc(sizeof(NezRect_f) * (count + cellArea[3] * cellArea[3]));
        HitboxListFromGrid(grid, cellArea, &colList.rect, &tileCount);
    }
    else{
        colList.rect = malloc(sizeof(NezRect_f) * count);
    }
    colList.size = count + tileCount;
    // insert bodies
    for (int i = 0; i < count; i++){
        colList.rect[tileCount + i] = bodies[i];
    }
    
    char flags = HitboxCollisionResolve(rect, velocity, &colList);
    
    if(colList.rect){
        free(colList.rect);
    }
    return flags;
}


NezRect_f HitboxExpand(NezRect_f *rect, NezVec2_f *size){
    return (NezRect_f){
        size->x > 0 ? rect->x : rect->x + size->x,
        size->y > 0 ? rect->y : rect->y + size->y,
        size->x > 0 ? rect->w + size->x : rect->w - size->x,
        size->y > 0 ? rect->h + size->y : rect->h - size->y
        };
}

void HitboxGridArea(NezRect_f *rect, NezGridMap *grid, int *areaOut){
    float offX = rect->x - grid->x;
    float offY = rect->y - grid->y;
    float offXw = rect->x - grid->x + rect->w;
    float offYh = rect->y - grid->y + rect->h;
    // compensate flooring
    if (offX < 0.0f){offX -= grid->s;}
    if (offY < 0.0f){offY -= grid->s;}
    if (offXw < 0.0f){offXw -= grid->s;}
    if (offYh < 0.0f){offYh -= grid->s;}
    
    // grid coordinates
    int X = (int)(offX / grid->s);
    int Y = (int)(offY / grid->s);
    int sizeX = (int)(offXw / grid->s) - X + 1;
    int sizeY = (int)(offYh / grid->s) - Y + 1;
    areaOut[0] = X;
    areaOut[1] = Y;
    areaOut[2] = sizeX;
    areaOut[3] = sizeY;
}
    
/*
    *outList = malloc(sizeof(NezRect_f) * sizeX * sizeY);
    *outCount = 0;
*/
void HitboxListFromGrid(NezGridMap *grid, int *area, NezRect_f **outList, int *outCount){
    int X = area[0];
    int Y = area[1];
    int sizeX = area[2];
    int sizeY = area[3];
    for (int y = Y; y < Y + sizeY; y++){
        if (y >= 0 && y < grid->h){
            for (int x = X; x < X + sizeX; x++){
                if (x >= 0 && x < grid->w){
                    char tile = grid->cell[x + y * grid->w];
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

char HitboxCollisionResolve(NezRect_f *rect, NezVec2_f *velocity, RectList *list){
    NezRect_f *a = rect;
    NezRect_f *b;
    NezRect_f c = (NezRect_f){a->x + velocity->x, a->y, a->w, a->h};
    char flags = 0;
    
    // Solve X axis separately
    for (int i = 0; i < list->size; i++){
        b = &list->rect[i]; // next collision Rectangle
        if (a != b && HitboxCheckHitbox(&c, b)) {
            // moving to the right
            if (velocity->x > 0.0f) {
                // adjust velocity 
                velocity->x = (b->x - a->w) - a->x;
                flags |= RIGHT_COL;
            }
            // moving to the left
            else if (velocity->x < 0.0f) {
                velocity->x = (b->x + b->w) - a->x;
                flags |= LEFT_COL;
            }
            flags |= X_COL;
        }
    }
    // set C to resolved X position
    c.x = a->x + velocity->x;
    
    // move on Y axis
    // set C on test Y position
    c.y += velocity->y;
    for (int i = 0; i < list->size; i++){
        b = &list->rect[i];
        if (a != b && HitboxCheckHitbox(&c, b)) {
            // moving down
            if (velocity->y > 0.0f) {
                velocity->y = (b->y - a->h) - a->y;
                flags |= BOTTOM_COL;
            }
            // moving up
            else if (velocity->y < 0.0f) {
                velocity->y = (b->y + b->h) - a->y;
                flags |= TOP_COL;
            }
            flags |= Y_COL;
        }
    }
    
    rect->x += velocity->x;
    rect->y += velocity->y;
    return flags;
}

// Ported from Javidx9 rectangle collision video
bool HitboxVsRaycast(NezVec2_f *rayOrig, NezVec2_f *rayDir, NezRect_f *target, NezVec2_f *colPoint, NezVec2_f *colNormal, float *timeStep){
    *colNormal = (NezVec2_f){0.0f, 0.0f};
    *colPoint = (NezVec2_f){0.0f, 0.0f};
    
    // Cache division
    NezVec2_f invdir = (NezVec2_f){1.0f / rayDir->x, 1.0f / rayDir->y};
    
    // Calculate intersections with rectangle bounding axes
    NezVec2_f t_near = (NezVec2_f){(target->x - rayOrig->x) * invdir.x, (target->y - rayOrig->y) * invdir.y};
    NezVec2_f t_far = (NezVec2_f){(target->x + target->w - rayOrig->x) * invdir.x, (target->y + target->h - rayOrig->y) * invdir.y};
    
    if (isnan(t_near.x) || isnan(t_near.y)){
        return false;}
    if (isnan(t_far.x) || isnan(t_far.y)){
        return false;}
    
    // Sort distance
    if (t_near.x > t_far.x) swapf(&t_near.x, &t_far.x);
    if (t_near.y > t_far.y) swapf(&t_near.y, &t_far.y);
    
    // Early rejection
    if (t_near.x > t_far.y || t_near.y > t_far.x) {return false;}
    
    // Closest 'time' will be the first contact
    *timeStep = fmaxf(t_near.x, t_near.y);
    
    // Furthest 'time' is contact on opposite side of target
    float t_hit_far = fminf(t_far.x, t_far.y);
    
    // Reject if ray direction is pointing away from object
    if (t_hit_far < 0){;return false;}
    
    // Contact point of collision from parametric line equation
    *colPoint = (NezVec2_f){rayOrig->x + *timeStep * rayDir->x, rayOrig->y + *timeStep * rayDir->y};
    
    if (t_near.x > t_near.y){
        if (invdir.x < 0){
            *colNormal = (NezVec2_f){ 1.0f, 0.0f };
        }else{
            *colNormal = (NezVec2_f){ -1.0f, 0.0f };
        }
    }else if (t_near.x < t_near.y){
        if (invdir.y < 0){
            *colNormal = (NezVec2_f){ 0.0f, 1.0f };
        }else{
            *colNormal = (NezVec2_f){ 0.0f, -1.0f };
        }
    }
    
    // Note if t_near == t_far, collision is principly in a diagonal
    // so pointless to resolve. By returning a CN={0,0} even though its
    // considered a hit, the resolver wont change anything.
    
    return true;
}

// Ported from Javidx9 rectangle collision video
bool HitboxDynamicVsHitbox(NezRect_f *movingRect, NezVec2_f *velocity, NezRect_f *target, NezVec2_f *colPoint, NezVec2_f *colNormal, float *timeStep){
    // Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
    if (velocity->x == 0.0f && velocity->y == 0.0f){return false;}
    
    // Expand target rectangle by source dimensions
    NezRect_f expanded_target = (NezRect_f){
        target->x - movingRect->w * 0.5,
        target->y - movingRect->h * 0.5,
        target->w + movingRect->w,
        target->h + movingRect->h,
    };
    NezVec2_f rayOrigin = {movingRect->x + movingRect->w * 0.5, movingRect->y + movingRect->h * 0.5};
    NezVec2_f rayDir = {velocity->x, velocity->y};
    
    
    if (HitboxVsRaycast(&rayOrigin, &rayDir, &expanded_target, colPoint, colNormal, timeStep)){
        return (*timeStep >= 0.0f && *timeStep < 1.0f);
    }
    else{
        return false;
    }
}

// Ported from Javidx9 rectangle collision video
bool HitboxResolveDynamicRectVsRect(NezRect_f *movingRect, NezVec2_f *velocity, NezRect_f *target){
    NezVec2_f colPoint;
    NezVec2_f colNormal;
    float timeStep = 0.0f;
    if (HitboxDynamicVsHitbox(movingRect, velocity, target, &colPoint, &colNormal, &timeStep)){
        velocity->x += colNormal.x * (float)fabs(velocity->x) * (1 - timeStep);
        velocity->y += colNormal.y * (float)fabs(velocity->y) * (1 - timeStep);
        return true;
    }
    return false;
}

/* // NOT USED
// bool HitboxAxisCollisionCheck(NezRect_f *a, NezRect_f *b, float *dirX, float *dirY) {
    // float d = (float)sqrt(*dirX * *dirX + *dirY * *dirY);
    // float projX = -*dirY / d;
    // float projY = *dirX / d;
    
    // float min_r1 = INFINITY;
    // float max_r1 = -INFINITY;
    // float a0 = ((a->x) * projX + a->y * (projY));
    // float a1 = ((a->x + a->w) * projX + a->y * projY);
    // float a2 = ((a->x + a->w) * projX + (a->y + a->h) * projY);
    // float a3 = ((a->x) * projX + (a->y + a->h) * projY);
    // min_r1 = fminf(min_r1, a0); max_r1 = fmaxf(max_r1, a0);
    // min_r1 = fminf(min_r1, a1); max_r1 = fmaxf(max_r1, a1);
    // min_r1 = fminf(min_r1, a2); max_r1 = fmaxf(max_r1, a2);
    // min_r1 = fminf(min_r1, a3); max_r1 = fmaxf(max_r1, a3);
    
    // float min_r2 = INFINITY;
    // float max_r2 = -INFINITY;
    // float b0 = ((b->x) * projX + b->y * (projY));
    // float b1 = ((b->x + b->w) * projX + b->y * projY);
    // float b2 = ((b->x + b->w) * projX + (b->y + b->h) * projY);
    // float b3 = ((b->x) * projX + (b->y + b->h) * projY);
    // min_r2 = fminf(min_r2, b0); max_r2 = fmaxf(max_r2, b0);
    // min_r2 = fminf(min_r2, b1); max_r2 = fmaxf(max_r2, b1);
    // min_r2 = fminf(min_r2, b2); max_r2 = fmaxf(max_r2, b2);
    // min_r2 = fminf(min_r2, b3); max_r2 = fmaxf(max_r2, b3);
    
    // if (!(max_r2 >= min_r1 && max_r1 >= min_r2)){
        // return false;
    // }else{
        // return true;
    // }
// }
*/

#endif //HITBOX_COLLISIONS_IMPLEMENTATION

