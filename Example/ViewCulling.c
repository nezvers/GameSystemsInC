/*******************************************************************************************
*
*   raylib [audio] example - Module playing (streaming)
*
*   This example has been created using raylib 1.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/


#include "raylib.h"
#define NEZ_TILESET_IMPLEMENTATION
#include "../tileset.h"
#define NEZ_TILEMAP_IMPLEMENTATION
#include "../tilemap.h"

int screenWidth = 480;
int screenHeight = 240;

// fake camera
typedef struct{
    float x;
    float y;
    int w;
    int h;
}View;

void InitGame(void);
void GameLoop(void);     // Update and Draw one frame
void (*Screen)(void);
void DefaultScreen(void);
void Inputs(void);

TileSet *tileSet;
TileMap *tileMap;
View cam = {0, 0, 16*5, 16*3};

int main(){
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "TileMap system test");
	
	
	
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(GameLoop, 0, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    InitGame();
    // Main game loop
    while (!WindowShouldClose()){    // Detect window close button or ESC key
        GameLoop();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    TileSetDestroy(tileSet);
    TileMapDestroy(tileMap);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}

void InitGame(void){
    tileSet = TileSetNewInitFromFile("../Resource/TileSetter_template.png", 16, 16);
    tileMap = TileMapNew();
    tileMap->tileSet = tileSet;
    tileMap->x = 100;
    tileMap->y = 50;
    TileMapInitSize(tileMap, 10, 5);
    for (int i=0; i<50; i++){tileMap->grid[i] = i;}
    
	Screen = DefaultScreen;
}



void GameLoop(void){
	Screen();
}


void DefaultScreen(){
    Inputs();
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);
        TileMapDrawGrid(tileMap);
        // only draw tiles that camera can see
        TileMapDrawExWorld(tileMap, (int)cam.x, (int)cam.y, cam.w, cam.h);
        DrawRectangleLines(cam.x, cam.y, cam.w, cam.h, LIGHTGRAY);
    EndDrawing();
}


// move fake camera
void Inputs(){
    cam.x += ((int)(IsKeyDown(KEY_D)||IsKeyDown(KEY_RIGHT)) - (int)(IsKeyDown(KEY_A)||IsKeyDown(KEY_LEFT))) *0.5;
    cam.y += ((int)(IsKeyDown(KEY_S)||IsKeyDown(KEY_DOWN)) - (int)(IsKeyDown(KEY_W)||IsKeyDown(KEY_UP))) *0.5;
}



















