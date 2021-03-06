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
#include "stdio.h"

#define NEZ_AUTOTILE_IMPLEMENTATION
#include "../autotile.h"
#define NEZ_TILEMAP_IMPLEMENTATION
#include "../tilemap.h"
#define NEZ_TILESET_IMPLEMENTATION
#include "../tileset.h"
#include "../Resource/TileSetBitmask.h"

int screenWidth = 480;
int screenHeight = 240;

void InitGame(void);
void GameLoop(void);     // Update and Draw one frame
void (*Screen)(void);
void DefaultScreen(void);
void Inputs(void);

TileSet *tileSet;
TileMap *tileMap;
AutoTile *autoTile;

int main(){
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "AutoTile system test");
	
	
	
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
    //tileSet = TileSetNewInitFromFile("../Resource/TileSetter_template.png", 16, 16);
    tileSet = TileSetNewInitFromFile("../Resource/grass_and_dirt_tiles.png", 16, 16);
    
    tileMap = TileMapNew();
    tileMap->tileSet = tileSet;
    TileMapInitSize(tileMap, 20, 10);
    autoTile = AutoTileNewInit(tileMap, 0, 5, 10, 5);
    AutoTileSetBitmaskData(autoTile, (int*)&bitmaskData, sizeof(bitmaskData));
    
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
        TileMapDrawGrid(tileMap, BLACK);
        TileMapDraw(tileMap);
        //TileMapDrawExWorld(tileMap, 0, 0, 20, 10);
    EndDrawing();
}


void Inputs(){
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        Vector2 mouse = GetMousePosition();
        int x = (int)(mouse.x-tileMap->x) / tileSet->tileX;
        int y = (int)(mouse.y-tileMap->y) / tileSet->tileY;
        if (mouse.x < tileMap->x){x--;}
        if (mouse.y < tileMap->y){y--;}
        AutoTileSetCellResize(autoTile, x, y);
    }
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
        Vector2 mouse = GetMousePosition();
        int x = (int)(mouse.x-tileMap->x) / tileSet->tileX;
        int y = (int)(mouse.y-tileMap->y) / tileSet->tileY;
        if (mouse.x < tileMap->x){x--;}
        if (mouse.y < tileMap->y){y--;}
        AutoTileRemoveCellResize(autoTile, x, y);
    }
    if(IsKeyPressed(KEY_RIGHT)){
        TileMapResize(tileMap, 0, 0, +1, 0);
    }
    else if(IsKeyPressed(KEY_LEFT)){
        TileMapResize(tileMap, 0, 0, -1, 0);
    }
    else if(IsKeyPressed(KEY_D)){
        TileMapResize(tileMap, +1, 0, 0, 0);
    }
    else if(IsKeyPressed(KEY_A)){
        TileMapResize(tileMap, -1, 0, 0, 0);
    }
    else if(IsKeyPressed(KEY_DOWN)){
        TileMapResize(tileMap, 0, 0, 0, +1);
    }
    else if(IsKeyPressed(KEY_UP)){
        TileMapResize(tileMap, 0, 0, 0, -1);
    }
}



















