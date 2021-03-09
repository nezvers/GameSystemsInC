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

#include <stdio.h>
#include "raylib.h"
#define NEZ_TILESET_IMPLEMENTATION
#include "../tileset.h"

int screenWidth = 480;
int screenHeight = 240;


void InitGame(void);
void GameLoop(void);        // Update and Draw one frame
void (*Screen)(void);       // Pointer for callback functions
void DefaultScreen(void);

TileSet *tileSet;

int main(){
    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
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
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}


RenderTexture2D texture;
void BakeTileSet(void);
void UpdateVariables(void);
int scale = 2;
int col;
int row;
int width;
int height;
Rectangle area = { 0 };
Camera2D camera;
int *bitmask;

void InitGame(void){
    
    tileSet = TileSetNewInitFromFile("../Resource/TileSetter_template.png", 16, 16);
    texture = LoadRenderTexture((tileSet->collumns*tileSet->tileX), (tileSet->rows*tileSet->tileY));
    col = tileSet->collumns;
    row = tileSet->rows;
    bitmask = MemAlloc(sizeof(int) * col * row);
    // set all to -1
    for (int i=0; i< col*row; i++){ bitmask[i] = -1;}
    
    camera = (Camera2D){ 0 };
    camera.target = (Vector2){ 0.0f,0.0f } ;
    UpdateVariables();
    
    BakeTileSet();
    
	Screen = DefaultScreen;
}



void GameLoop(void){
	Screen();
}

void Input(void);
void DrawTileHighlight(void);

void DefaultScreen(){
    Input();
    
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);
        BeginMode2D(camera);
            DrawTextureRec(texture.texture, (Rectangle){ 0.0f, 0.0f,
                       (float)texture.texture.width, (float)-texture.texture.height },
                       (Vector2){0.0f,0.0f}, WHITE);
        EndMode2D();
        
        DrawTileHighlight();
        
        DrawText("Mark direction for neighbour tiles", 20, screenHeight - 20, 20, LIGHTGRAY);
        
    EndDrawing();
}

void BakeTileSet(){
    BeginTextureMode(texture);
    for(int y = 0; y < row; y++){
        for(int x = 0; x < col; x++){
            TileSetDrawTile(tileSet, x+y*col, x*tileSet->tileX, y*tileSet->tileY);
        }
    }
    EndTextureMode();
}


Vector2 mouseCurrent;
Vector2 mouseStart;
Vector2 camStartPos; // needed to move by integer values;
Vector2 mouseTile;
bool drag = false;
bool inArea = false;
int bit[] = {1, 2, 4, 8, 0, 16, 32, 64, 128}; // center is 0 and is considered as active
void PrintBitmaskData(void);
void Input(void){
    if (IsWindowResized()){
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        UpdateVariables();
    }
    if (IsKeyPressed(KEY_ENTER)){PrintBitmaskData();}
    int mWheel = GetMouseWheelMove();
    if (mWheel != 0){
        scale += mWheel;
        scale = scale > 0 ? scale : 1;
        UpdateVariables();
    }
    mouseCurrent = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)){
        drag = true;
        mouseStart = mouseCurrent;
        camStartPos = camera.target;
    }
    if (IsMouseButtonReleased(MOUSE_MIDDLE_BUTTON)){
        drag = false;
    }
    if (drag){
        // need to move by int without float rounding loss, hence camStartPos
        camera.target.x = camStartPos.x +(int)(( mouseStart.x -mouseCurrent.x) /scale);
        camera.target.y = camStartPos.y +(int)(( mouseStart.y -mouseCurrent.y) /scale);
    }
    
    // Offset after moving camera
    mouseTile.x = mouseCurrent.x + (camera.target.x *scale);
    mouseTile.y = mouseCurrent.y + (camera.target.y *scale);
    inArea = !(mouseTile.x < 0|| mouseTile.y < 0 || mouseTile.x > area.width -1 || mouseTile.y > area.height -1);
    if (!inArea){return;}
    
    //no button pressed
    if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
        return;
    }
    
    int w = tileSet->tileX *scale;
    int h = tileSet->tileY *scale;
    int x = ((int)mouseTile.x / w);
    int y = ((int)mouseTile.y / h);
    int posX = x * w;
    int posY = y * h;
    int id = x+y*col;
    
    int mx = ((int)mouseTile.x - posX) / (w/3);
    int my = ((int)mouseTile.y - posY) / (h/3);
    // Floating point imprecision on positive direction edges
    mx = mx > 2 ? 2 : mx;
    my = my > 2 ? 2 : my;
    int b = bit[mx +my *3];
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        if (bitmask[id] < 0){bitmask[id] = 0;}
        bitmask[id] |= b;
    }
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
        if (bitmask[id] == 0 && b == 0){bitmask[id] = -1;}
        else{bitmask[id] &= ~b;}
    }
}

void UpdateVariables(void){
    //Vector2 center = (Vector2){ screenWidth/(2.0f *scale), screenHeight/(2.0f *scale) };
    
    area.width = (tileSet->tileX * col) *scale;
    area.height = (tileSet->tileY * row) *scale;
    
    camera.offset = (Vector2){ 0.0f,0.0f };
    camera.zoom = (float)scale;
}


Color colorBit = (Color){ 100, 175, 241, 100 };
Color colorEmpty = (Color){ 241, 175, 50, 100 };
Color colorHover = LIGHTGRAY;

void DrawTileHighlight(void){
    
    int w = (int)tileSet->tileX *scale;
    int h = (int)tileSet->tileY *scale;
    
    int ws = w/3;
    int hs = h/3;
    int offX = (int)-camera.target.x *scale;
    int offY = (int)-camera.target.y *scale;
    for (int y = 0; y < row; y++){
        for (int x = 0; x < col; x++){
            int id = x+y*col;
            int mask = bitmask[id];
            if(mask > -1){
                for (int i=0; i<9; i++){
                    int ys = i/3;
                    int xs = i%3;
                    int b = bit[i];
                    if (mask & b){
                        DrawRectangle(x*w +ws*xs +offX, y*h +hs*ys +offY, ws, hs, colorBit);
                    }
                }
                DrawRectangle(x*w +ws*1 +offX, y*h +hs*1 +offY, ws, hs, colorBit);
            }
            else{
                DrawRectangle(x*w +offX, y*h +offY, w, h, colorEmpty);
            }
        }
    }
    
    
    
    //Hovered tile
    if (!inArea){return;}
    int x = (int)(mouseTile.x / w);
    int y = (int)(mouseTile.y / h);
    int posX = x * w;
    int posY = y * h;
    DrawRectangleLines( posX  -1 +offX, posY -1 +offY, w +2, h +2, colorHover);
    
    
}

// tiles  without bitmask has value of -1 and shouldn't be processed when importing.
void PrintBitmaskData(void){
    printf("int bitmaskData[] = {");
    for (int i = 0; i < tileSet->tileCount; i++){
        printf("%d, ", bitmask[i]);
    }
    printf("};\n");
}












