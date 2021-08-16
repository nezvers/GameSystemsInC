

#include "raylib.h"
#include "math.h"

#define HITBOX_COLLISIONS_IMPLEMENTATION
#include "hitbox_collision.h"

// testing forward declaring
bool HitboxMoveCollisionCheck(NezRect_f *a, NezRect_f *b, float *spdX, float *spdY);

const int screenWidth = 800;
const int screenHeight = 600;

void GameInit();
void GameUpdate();
void GameDraw();

NezRect_f rect1 = {300.0f, 400.0f, 32.0f, 32.0f};
NezRect_f rect2 = {400.0f, 400.0f, 32.0f, 32.0f};
bool isColliding = false;
Vector2 mouse = {};



int main(void){
    GameInit();
    SetTargetFPS(60);
    while (!WindowShouldClose()){
        GameUpdate();
        GameDraw();
    }
    CloseWindow();
    return 0;
}

void GameInit() {
    InitWindow(screenWidth, screenHeight, "raylib [models] example - geometric shapes");
}


void GameUpdate(){
    mouse = GetMousePosition();
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        rect1.x = mouse.x;
        rect1.y = mouse.y;
    }
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
        rect2.x = mouse.x;
        rect2.y = mouse.y;
    }
    float x = mouse.x - rect1.x;
    float y = mouse.y - rect1.y;
    isColliding = HitboxMoveCollisionCheck(&rect1, &rect2, &x, &y);
}


void GameDraw(){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    
    DrawRectangle((int)rect1.x, (int)rect1.y, (int)rect1.w, (int)rect1.h, isColliding ? RED : BLUE);
    
    DrawRectangle((int)rect2.x, (int)rect2.y, (int)rect2.w, (int)rect2.h, GREEN);
    
    DrawLine((int)rect1.x, (int)rect1.y, (int)mouse.x, (int)mouse.y, BLACK);

    EndDrawing();
}
