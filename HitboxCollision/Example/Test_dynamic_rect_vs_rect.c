

#include "raylib.h"
#include "math.h"

#define HITBOX_COLLISIONS_IMPLEMENTATION
#include "hitbox_collision.h"


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
    //NezVec2_f rayOrig = {rect1.x, rect1.y};
    NezVec2_f rayDir = {mouse.x - rect1.x, mouse.y - rect1.y};
    NezVec2_f cp;
    NezVec2_f cn;
    float t;
    isColliding = HitboxDynamicVsHitbox(&rect1, &rayDir, &rect2, &cp, &cn, &t);
    // printf("Time of impact: %.4f\n", t);
    // printf("Colision point: (%.2f, %.2f)\n", cp.x, cp.y);
    // printf("Colision normal: (%.2f, %.2f)\n", cn.x, cn.y);
}


void GameDraw(){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    
    DrawRectangle((int)rect1.x, (int)rect1.y, (int)rect1.w, (int)rect1.h, isColliding ? RED : BLUE);
    
    DrawRectangle((int)rect2.x, (int)rect2.y, (int)rect2.w, (int)rect2.h, GREEN);
    
    DrawLine((int)rect1.x, (int)rect1.y, (int)mouse.x, (int)mouse.y, BLACK);

    EndDrawing();
}
