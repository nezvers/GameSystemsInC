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
        HitboxMoveAndCollideHitbox(NezRect_f *a, NezRect_f *b, float *spdX, float *spdY);



#ifdef __cplusplus
}
#endif
#endif //HITBOX_COLLISIONS_H

#ifdef HITBOX_COLLISIONS_IMPLEMENTATION
#undef HITBOX_COLLISIONS_IMPLEMENTATION

bool HitboxCheckHitbox(NezRect_f *a, NezRect_f *b) {
    return  a->x < b->x + b->w && a->x + a->w > b->x && a->y< b->y + b->h && a->y + a->h > b->y;
}


void HitboxMoveAndCollideHitbox(NezRect_f *a, NezRect_f *b, float *spdX, float *spdY) {
    NezRect_f c = (NezRect_f){a->x + *spdX, a->y, a->w, a->h};

    if (HitboxCheckHitbox(&c, b)) {
        if (*spdX > 0.0f) {
            *spdX = (b->x - a->w) - a->x;
        }
        else if (*spdX < 0.0f) {
            *spdX = (b->x + b->w) - a->x;
        }
    }
    c.x = a->x + *spdX;
    c.y += *spdY;

    if (HitboxCheckHitbox(&c, b)) {
        if (*spdY > 0.0f) {
            *spdY = (b->y - a->h) - a->y;
        }
        else if (*spdY < 0.0f) {
            *spdY = (b->y + b->h) - a->y;
        }
    }
}

// Test if movement direction can result with collision
bool HitboxMoveCollisionCheck(NezRect_f *a, NezRect_f *b, float *spdX, float *spdY) {
    float d = (float)sqrt(*spdX * *spdX + *spdY * *spdY);
    float projX = -*spdY / d;
    float projY = *spdX / d;
    
    float min_r1 = INFINITY;
    float max_r1 = -INFINITY;
    float a0 = ((a->x) * projX + a->y * (projY));
    float a1 = ((a->x + a->w) * projX + a->y * projY);
    float a2 = ((a->x + a->w) * projX + (a->y + a->h) * projY);
    float a3 = ((a->x) * projX + (a->y + a->h) * projY);
    min_r1 = fminf(min_r1, a0); max_r1 = fmaxf(max_r1, a0);
    min_r1 = fminf(min_r1, a1); max_r1 = fmaxf(max_r1, a1);
    min_r1 = fminf(min_r1, a2); max_r1 = fmaxf(max_r1, a2);
    min_r1 = fminf(min_r1, a3); max_r1 = fmaxf(max_r1, a3);
    
    float min_r2 = INFINITY;
    float max_r2 = -INFINITY;
    float b0 = ((b->x) * projX + b->y * (projY));
    float b1 = ((b->x + b->w) * projX + b->y * projY);
    float b2 = ((b->x + b->w) * projX + (b->y + b->h) * projY);
    float b3 = ((b->x) * projX + (b->y + b->h) * projY);
    min_r2 = fminf(min_r2, b0); max_r2 = fmaxf(max_r2, b0);
    min_r2 = fminf(min_r2, b1); max_r2 = fmaxf(max_r2, b1);
    min_r2 = fminf(min_r2, b2); max_r2 = fmaxf(max_r2, b2);
    min_r2 = fminf(min_r2, b3); max_r2 = fmaxf(max_r2, b3);
    
    if (!(max_r2 >= min_r1 && max_r1 >= min_r2)){
        return false;
    }else{
        return true;
    }
}

#endif //HITBOX_COLLISIONS_IMPLEMENTATION
