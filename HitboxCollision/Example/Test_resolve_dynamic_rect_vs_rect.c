

#include "raylib.h"
#include "math.h"

#define HITBOX_COLLISIONS_IMPLEMENTATION
#include "hitbox_collision.h"


const int screenWidth = 800;
const int screenHeight = 600;

void GameInit();
void GameUpdate();
void GameDraw();

NezRect_f rect1 = {400.0f, 300.0f, 32.0f, 32.0f};
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
    static const float maxSpd = 6.0f;
    static int dirX = 0;
    static int dirY = 0;
    static NezVec2_f vel = {0};
    static const float acc = 0.1;
    
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        Vector2 mouse = GetMousePosition();
        rect1.x = mouse.x;
        rect1.y = mouse.y;
    }
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
        Vector2 mouse = GetMousePosition();
        rect2.x = mouse.x;
        rect2.y = mouse.y;
    }
    
    // INPUT DIRECTION
    dirX = (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    dirY = (float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W));
    
    // HORIZONTAL SPEED
    vel.x += (dirX * maxSpd - vel.x) * acc;
    if (vel.x < -maxSpd){
        vel.x = -maxSpd;
    }
    else if (vel.x > maxSpd){
        vel.x = maxSpd;
    }
    
    // VERTICAL SPEED
    vel.y += (dirY * maxSpd - vel.y) * acc;
    if (vel.y < -maxSpd){
        vel.y = -maxSpd;
    }
    else if (vel.y > maxSpd){
        vel.y = maxSpd;
    }
    isColliding = HitboxResolveDynamicRectVsRect(&rect1, &vel, &rect2);
    
    rect1.x += vel.x;
    rect1.y += vel.y;
}


void GameDraw(){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    DrawRectangle((int)rect1.x, (int)rect1.y, (int)rect1.w, (int)rect1.h, isColliding ? RED : BLUE);
    
    DrawRectangle((int)rect2.x, (int)rect2.y, (int)rect2.w, (int)rect2.h, GREEN);

    EndDrawing();
}
