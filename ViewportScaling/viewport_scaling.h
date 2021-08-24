/*
    
*/

#ifndef VIEWPOR_SCALING_H
#define VIEWPOR_SCALING_H


#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "math.h"


#if defined RAYLIB_H
    #define NezRect_f Rectangle
    #define w width
    #define h height
#else
    #ifndef NEZRECT_F
    #define NEZRECT_F
    typedef struct {
        float x;    // origin x
        float y;    // origin y
        float w;    // width
        float h;    // height
    } NezRect_f;
    #endif // NEZRECT_F
#endif

#if defined RAYLIB_H
    #define NezVec2_f Vector2
#else
    #ifndef NEZVEC2_F
    #define NEZVEC2_F
    typedef struct {
        float x;
        float y;
    } NezVec2_f;
    #endif // NEZVEC2_F
#endif

#ifndef NEZVPAPI
    #ifdef NEZ_VIEWPORT_SCALING_STATIC
        #define NEZVPAPI static
    #else
        #define NEZVPAPI extern
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
    // Helper function to get available scale
    NEZVPAPI int
    ViewportGetAspectScalePixel(int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight);
    NEZVPAPI float
    ViewportGetAspectScale(int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight);
    // Get rectangles for scaling viewport to the window
    NEZVPAPI void
    ViewportKeepAspectPixel(NezRect_f *view, NezRect_f *screen, int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight);
    NEZVPAPI void
    ViewportKeepHeightPixel(NezRect_f *view, NezRect_f *screen, int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight);
    NEZVPAPI void
    ViewportKeepWidthPixel(NezRect_f *view, NezRect_f *screen, int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight);
    NEZVPAPI void
    ViewportKeepAspect(NezRect_f *view, NezRect_f *screen, int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight);
    NEZVPAPI void
    ViewportKeepHeight(NezRect_f *view, NezRect_f *screen, int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight);
    NEZVPAPI void
    ViewportKeepWidth(NezRect_f *view, NezRect_f *screen, int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight);
    
#ifdef __cplusplus
}
#endif
#endif //VIEWPOR_SCALING_H

#ifdef VIEWPOR_SCALING_IMPLEMENTATION
#undef VIEWPOR_SCALING_IMPLEMENTATION

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

int ViewportGetAspectScalePixel(int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight){
    return MAX(1, MIN((*windowWidth / *viewWidth), (*windowHeight / *viewHeight)));
}

float ViewportGetAspectScale(int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight){
    return MAX(1, MIN(((float)*windowWidth / *viewWidth), ((float)*windowHeight / *viewHeight)));
}

void ViewportKeepAspectPixel(NezRect_f *view, NezRect_f *screen, int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight){
    int scale = MAX(1, MIN((*windowWidth / *viewWidth), (*windowHeight / *viewHeight)));
    view->w = *viewWidth;
    view->h = *viewHeight;
    view->x = 0.0f;
    view->y = 0.0f;
    
    screen->w = (int)view->w * scale;
    screen->h = (int)view->h * scale;
    screen->x = (*windowWidth - screen->w) / 2;
    screen->y = (*windowHeight - screen->h) / 2;
}

void ViewportKeepHeightPixel(NezRect_f *view, NezRect_f *screen, int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight){
    int scale = MAX(1, (*windowHeight / *viewHeight));
    view->w = *windowWidth / scale;
    view->h = *viewHeight;
    view->x = 0.0f;
    view->y = 0.0f;
    
    screen->w = (int)view->w * scale;
    screen->h = (int)view->h * scale;
    screen->x = (*windowWidth - screen->w) / 2;
    screen->y = (*windowHeight - screen->h) / 2;
}

void ViewportKeepWidthPixel(NezRect_f *view, NezRect_f *screen, int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight){
    int scale = MAX(1, (*windowWidth / *viewWidth));
    view->w = *viewWidth;
    view->h = *windowHeight / scale;
    view->x = 0.0f;
    view->y = 0.0f;
    
    screen->w = (int)view->w * scale;
    screen->h = (int)view->h * scale;
    screen->x = (*windowWidth - screen->w) / 2;
    screen->y = (*windowHeight - screen->h) / 2;
}

void ViewportKeepAspect(NezRect_f *view, NezRect_f *screen, int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight){
    float scale = MAX(1.0f, MIN(((float)*windowWidth / *viewWidth), ((float)*windowHeight / *viewHeight)));
    view->w = *viewWidth;
    view->h = *viewHeight;
    view->x = 0.0f;
    view->y = 0.0f;
    
    screen->w = view->w * scale;
    screen->h = view->h * scale;
    screen->x = (*windowWidth - screen->w) / 2;
    screen->y = (*windowHeight - screen->h) / 2;
}

void ViewportKeepHeight(NezRect_f *view, NezRect_f *screen, int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight){
    float scale = MAX(1.0f, ((float)*windowHeight / *viewHeight));
    view->w = (float)*windowWidth / scale;
    view->h = *viewHeight;
    view->x = 0.0f;
    view->y = 0.0f;
    
    screen->w = view->w * scale;
    screen->h = view->h * scale;
    screen->x = (*windowWidth - screen->w) / 2;
    screen->y = (*windowHeight - screen->h) / 2;
}

void ViewportKeepWidth(NezRect_f *view, NezRect_f *screen, int *viewWidth, int *viewHeight, int *windowWidth, int *windowHeight){
    float scale = MAX(1.0f, ((float)*windowWidth / *viewWidth));
    view->w = *viewWidth;
    view->h = (float)*windowHeight / scale;
    view->x = 0.0f;
    view->y = 0.0f;
    
    screen->w = view->w * scale;
    screen->h = view->h * scale;
    screen->x = (*windowWidth - screen->w) / 2;
    screen->y = (*windowHeight - screen->h) / 2;
}

#endif //VIEWPOR_SCALING_IMPLEMENTATION

