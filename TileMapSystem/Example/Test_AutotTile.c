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
TileMap *tileMap1;
TileMap *tileMap2;
AutoTile *autoTile;
AutoTile *autoTile1;
AutoTile *autoTile2;

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
    TileMapDestroy(tileMap1);
    TileMapDestroy(tileMap2);
    AutoTileDestroy(autoTile1);
    AutoTileDestroy(autoTile2);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}

void InitGame(void){
    //tileSet = TileSetNewInitFromFile("../Resource/TileSetter_template.png", 16, 16);
    tileSet = TileSetNewInitFromFile("../Resource/grass_and_dirt_tiles.png", 16, 16, NULL, 0);
    
    // tilemap for each autoTile
    tileMap1 = TileMapNew();
    tileMap1->tileSet = tileSet;
    tileMap2 = TileMapNew();
    tileMap2->tileSet = tileSet;
    
    autoTile1 = AutoTileNewInitTileRegion(tileMap1, 0, 0, 10, 5);
    AutoTileSetBitmaskData(autoTile1, (int*)&bitmaskData, sizeof(bitmaskData)/ sizeof(bitmaskData[0]));
    
    // create AutoTile providing list of TileSet IDs
    int tileList[] = {50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};
    autoTile2 = AutoTileNewInitTileList(tileMap2, (int*)&tileList, sizeof(tileList)/ sizeof(tileList[0]));
    AutoTileSetBitmaskData(autoTile2, (int*)&bitmaskData, sizeof(bitmaskData)/ sizeof(bitmaskData[0]));
    
    autoTile = autoTile1;
    
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
        TileMapDrawGrid(tileMap1, BLACK);
        TileMapDrawGrid(tileMap2, BLACK);
        TileMapDraw(tileMap2); //dirt
        TileMapDraw(tileMap1); //grass
        //TileMapDrawExWorld(tileMap, 0, 0, 20, 10);
    EndDrawing();
}


void Inputs(){
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        Vector2 mouse = GetMousePosition();
        AutoTileSetCellResizeWorld(autoTile, (int)mouse.x, (int)mouse.y);
    }
    else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
        Vector2 mouse = GetMousePosition();
        AutoTileRemoveCellResizeWorld(autoTile, (int)mouse.x, (int)mouse.y);
    }
    // change active AutoTile
    if(IsKeyPressed(KEY_ONE)){
        autoTile = autoTile1;
    }
    else if(IsKeyPressed(KEY_TWO)){
        autoTile = autoTile2;
    }
}


















