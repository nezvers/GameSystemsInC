

#include "raylib.h"
#include "math.h"

#define HITBOX_COLLISIONS_IMPLEMENTATION
#include "hitbox_collision.h"

const int screenWidth = 800;
const int screenHeight = 600;

void GameInit();
void GameUpdate();
void GameDraw();

#define SOLID_COUNT 6
NezRect_f solids[] = {
    {192.0f, 454.0f, 96.0f, 32.0f},
    {384.0f, 358.0f, 128.0f, 32.0f},
    {64.0f, 268.0f, 96.0f, 32.0f},
    {0.0f, 550.0f, 800.0f, 50.0f},// floor
    {-18.0f, 0.0f, 32.0f, 600.0f},// left wall
    {782.0f, 0.0f, 32.0f, 600.0f},// right wall
};
NezRect_f player = {100.0f, 100.0f, 32.0, 32.0f};



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
    // VARIABLES
    static const float maxX = 6.0f;
    static const float jumpSpd = -10.0f;
    static const float grav = 0.4f;
    static const float acc = 0.1;
    static float spdX = 0.0f;
    static float spdY = 0.0f;
    static bool isGrounded = false;
    static int dirX = 0;
    
    // INPUT DIRECTION
    dirX = (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    
    // HORIZONTAL SPEED
    spdX += (dirX * maxX - spdX) * acc;
    if (spdX < -maxX){
        spdX = -maxX;
    }
    else if (spdX > maxX){
        spdX = maxX;
    }
    
    // VERTICAL SPEED
    if (isGrounded && IsKeyPressed(KEY_SPACE)){
        spdY = jumpSpd;
    }
    else{
        spdY += grav;
    }
    if (spdY > -jumpSpd){
        spdY = -jumpSpd;
    }
    float prevY = spdY;
    
    // COLLISION
    for (int i = 0; i < SOLID_COUNT; i++){
        HitboxMoveAndCollideHitbox(&player, &solids[i], &spdX, &spdY);
    }
    
    // GROUND CHECK
    if (prevY > 0.0f && spdY <= 0.0f){
        isGrounded = true;
    }
    else{
        isGrounded = false;
    }
    
    // POSITION
    player.x += spdX;
    player.y += spdY;
}


void GameDraw(){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    // PLAYER
    DrawRectangle((int)player.x, (int)player.y, (int)player.w, (int)player.h, BLUE);
    
    // SOLIDS
    for (int i = 0; i < SOLID_COUNT; i++) {
        DrawRectangle((int)solids[i].x, (int)solids[i].y, (int)solids[i].w, (int)solids[i].h, RED);
    }

    EndDrawing();
}
