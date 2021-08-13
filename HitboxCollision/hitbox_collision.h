#ifndef HITBOX_COLLISIONS_H
#define HITBOX_COLLISIONS_H


#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

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

#endif //HITBOX_COLLISIONS_IMPLEMENTATION