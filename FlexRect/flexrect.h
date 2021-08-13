#ifndef NEZ_FLEXRECT_H
#define NEZ_FLEXRECT_H

#include "stddef.h"

#include "stdlib.h"
#include "stdio.h"

// Flags for expanding in case parent rect is smaller than minimal size
enum {
    FR_RIGHT    = 0, // grow rigth
    FR_LEFT     = 1, // grow left
    FR_H_CENTER = 2, // grow centered
    FR_DOWN     = 0, // grow down
    FR_UP       = 1, // grow up
    FR_V_CENTER = 2, // grow centered
};

#ifndef NEZRECT_I
#define NEZRECT_I
// Basic rectangle struct with x & y fosition and w & h size
typedef struct{
    int x;
    int y;
    int w;
    int h;
}NezRect_i;
#endif // NEZRECT_I

// Flexible rectangle that positions self depending on it's parameters
typedef struct FlexRect FlexRect;


#ifndef NEZFRAPI
    #ifdef NEZ_FLEXRECT_STATIC
        #define NEZFRAPI static
    #else
        #define NEZFRAPI extern
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Create new FlexRect providing NezRect_i rectangle, anchor percentage from sides and margin pixels from sides and minimal width & height.
NEZFRAPI FlexRect* 
FlexRectNew(NezRect_i *rect, float al, float at, float ar, float ab, int ml, int mt, int mr, int mb, int minw, int minh);
// Deallocate FlexRect and it's children
NEZFRAPI void
FlexRectDestroy(FlexRect *rect);
// Create child FlexRect that automatically links with parent FlexRect
NEZFRAPI FlexRect*
FlexRectCreateChild(FlexRect *parent, float al, float at, float ar, float ab, int ml, int mt, int mr, int mb, int minw, int minh);
// Remove parenting over child FlexRect
NEZFRAPI void
FlexRectRemoveChild(FlexRect *parent, FlexRect *child);
// Destroys FlexRect and it's children
NEZFRAPI void
FlexRectDestroyChild(FlexRect *parent, FlexRect *child);
// Give new rectangle are to resize it and it's children
NEZFRAPI void
FlexRectResize(FlexRect *fr, NezRect_i *rect);
// FlexRectSetFlags();
// FlexRectWithPoint(FlexRect *rect);

#ifdef __cplusplus
}
#endif
#endif // NEZ_FLEXRECT_H

// -------------------------------------

#ifdef NEZ_FLEXRECT_IMPLEMENTATION
#undef NEZ_FLEXRECT_IMPLEMENTATION

struct FlexRect{
    float al;        // anchor left
    float at;        // anchor top
    float ar;        // anchor right
    float ab;        // anchor bottom
    int ml;          // margin left
    int mt;          // margin top
    int mr;          // margin right
    int mb;          // margin bottom
    int minw;        // minimal width
    int minh;        // minimal height
    NezRect_i r;     // rectangle area
    int childCount;
    FlexRect **children;
    FlexRect *parent;
};

NezRect_i
FlexRectGetRect(NezRect_i *rect, float al, float at, float ar, float ab, int ml, int mt, int mr, int mb, int minw, int minh){
    int x = rect->x + (rect->w * al) + ml;
    int y = rect->y + (rect->h * at) + mt;
    int w = (rect->w - (rect->w * al) - ml - mr) * ar;
    int h = (rect->h - (rect->h * at) - mt - mb) * ab;
    if (w < minw){w = minw;}
    if (h < minh){h = minh;}
    NezRect_i r = {
        x,
        y,
        w,
        h,
    };
    return r;
}


FlexRect* 
FlexRectNew(NezRect_i *rect, float al, float at, float ar, float ab, int ml, int mt, int mr, int mb, int minw, int minh){
    NezRect_i r = FlexRectGetRect(rect, al, at, ar, ab, ml, mt, mr, mb, minw, minh);
    
    FlexRect* fr = malloc(sizeof(FlexRect));
    *fr = (FlexRect){
        al,        // anchor left
        at,        // anchor top
        ar,        // anchor right
        ab,        // anchor bottom
        ml,        // margin left
        mt,        // margin top
        mr,        // margin right
        mb,        // margin bottom
        minw,      // minimal width
        minh,      // minimal height
        r,
        0,
        NULL,
        NULL,
    };
    return fr;
}

void
FlexRectDestroy(FlexRect *rect){
    for (int i = 0; i < rect->childCount; i++){
        FlexRectDestroy(rect->children[i]);
    }
    free(rect);
}

FlexRect*
FlexRectCreateChild(FlexRect *parent, float al, float at, float ar, float ab, int ml, int mt, int mr, int mb, int minw, int minh){
    FlexRect ** old = parent->children;
    FlexRect *newChild = FlexRectNew(&parent->r, al, at, ar, ab, ml, mt, mr, mb, minw, minh);
    parent->children = malloc(sizeof(FlexRect**) * (parent->childCount + 1));
    for (int i = 0; i < parent->childCount; i++){
        parent->children[i] = old[i];
    }
    parent->children[parent->childCount] = newChild;
    
    if (parent->childCount > 0){
        free(old);
    }
    parent->childCount += 1;
    return newChild;
}

void
FlexRectRemoveChild(FlexRect *parent, FlexRect *child){
    if (parent->childCount < 1){
        return;
    }
    else if (parent->childCount == 1){
        free(parent->children);
        parent->childCount = 0;
        parent->children = 0;
        return;
    }
    FlexRect ** old = parent->children;
    parent->children = malloc(sizeof(FlexRect**) * (parent->childCount - 1));
    for (int i = 0; i < parent->childCount; i++){
        if (parent->children[i] != child){
            parent->children[i] = old[i];
        }
    }
    free(old);
    parent->childCount -= 1;
}

void
FlexRectDestroyChild(FlexRect *parent, FlexRect *child){
    FlexRectRemoveChild(parent, child);
    FlexRectDestroy(child);
}

void
FlexRectResize(FlexRect *fr, NezRect_i *rect){
    fr->r = FlexRectGetRect(rect, fr->al, fr->at, fr->ar, fr->ab, fr->ml, fr->mt, fr->mr, fr->mb, fr->minw, fr->minh);
    for(int i = 0 ; i < fr->childCount; i++){
        FlexRectResize(fr->children[i], &fr->r);
    }
}


#endif // NEZ_FLEXRECT_IMPLEMENTATION

