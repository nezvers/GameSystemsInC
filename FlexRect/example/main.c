/*******************************************************************************************
*
*   
*
********************************************************************************************/

#include "raylib.h"

#define NEZ_FLEXRECT_IMPLEMENTATION
#include "../flexrect.h"

//#define PLATFORM_WEB

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif


int screenWidth = 640;
int screenHeight = 360;

void InitGame(void);
void GameLoop(void);

FlexRect2 rect;

FlexRect *flexRect;
FlexRect *flexChild1;

void Resize();

//----------------------------------------------------------------------------------
int main(){
    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(GameLoop, 0, 1);
#else
    SetTargetFPS(60);
    
	while (!WindowShouldClose()){
        GameLoop();
    }
#endif
	
	
    CloseWindow();
    return 0;
}


void InitGame(){
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "FlexRect test");
    
    rect = (FlexRect2){0, 0, screenWidth, screenHeight};
    flexRect = FlexRectNew(&rect, 0.0f, 0.0f, 1.0f, 1.0f, 10, 10, 10, 10, 0, 0);
    flexChild1 = FlexRectCreateChild(flexRect, 0.0f, 0.0f, 1.0f, 0.2f, 10, 10, 10, 0, 300, 200);
}

void GameLoop(void){
	if (IsWindowResized()){
		screenWidth = GetScreenWidth();
		screenHeight = GetScreenHeight();
        //FlexRectDestroyChild(flexRect, flexChild1);
        //FlexRectDestroy(flexRect);
        Resize();
	}
    
	
	BeginDrawing();

        ClearBackground(RAYWHITE);
		
		if (flexRect){
            DrawRectangleLines(flexRect->r.x, flexRect->r.y, flexRect->r.w, flexRect->r.h, RED);
            DrawRectangleLines(flexChild1->r.x, flexChild1->r.y, flexChild1->r.w, flexChild1->r.h, RED);
        }
        
    EndDrawing();
}


void Resize(){
    rect = (FlexRect2){0, 0, screenWidth, screenHeight};
    //flexRect = FlexRectNew(&rect, 0.0f, 0.0f, 1.0f, 1.0f, 10, 10, 10, 10);
    //flexChild1 = FlexRectCreateChild(flexRect, 0.0f, 0.0f, 1.0f, 0.2f, 10, 10, 10, 0);
    FlexRectResize(flexRect, &rect);
}

