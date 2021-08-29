

#include "raylib.h"
#include "math.h"

#define HITBOX_COLLISIONS_IMPLEMENTATION
#include "hitbox_collision.h"


const int screenWidth = 800;
const int screenHeight = 600;

void GameInit();
void GameUpdate();
void GameDraw();

void DrawTileGrid();
void DrawTileMap();

#define MAP_W 20
#define MAP_H 12
#define SOLID_COUNT 6
NezRect_f solids[] = {
    {192.0f, 454.0f, 96.0f, 32.0f},
    {384.0f, 358.0f, 128.0f, 32.0f},
    {64.0f, 268.0f, 96.0f, 32.0f},
    {0.0f, 550.0f, 800.0f, 50.0f},// floor
    {-18.0f, 0.0f, 32.0f, 600.0f},// left wall
    {782.0f, 0.0f, 32.0f, 600.0f},// right wall
};
NezRect_f player = {32.0f * 2, 32.0f * 8, 32.0f, 32.0f};

NezGridMap map;
int tiles[] = {
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

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
    map.x = 0.0f;
    map.y = 0.0f;
    map.w = MAP_W;
    map.h = MAP_H;
    map.s = 32;
    map.cell = tiles;
}


void GameUpdate(){
    // VARIABLES
    static const float maxX = 6.0f;
    static const float jumpSpd = -10.0f;
    static const float grav = 0.4f;
    static const float acc = 0.1;
    static NezVec2_f vel = {0};
    static bool isGrounded = false;
    static int dirX = 0;
    
    // INPUT DIRECTION
    dirX = (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    
    // HORIZONTAL SPEED
    vel.x += (dirX * maxX - vel.x) * acc;
    if (vel.x < -maxX){
        vel.x = -maxX;
    }
    else if (vel.x > maxX){
        vel.x = maxX;
    }
    
    // VERTICAL SPEED
    if (isGrounded && IsKeyPressed(KEY_SPACE)){
        vel.y = jumpSpd;
    }
    else{
        vel.y += grav;
    }
    if (vel.y > -jumpSpd){
        vel.y = -jumpSpd;
    }
    float prevY = vel.y;
    
    // COLLISION
    //HitboxMoveAndCollide(&player, &vel, solids, SOLID_COUNT);
    HitboxMoveAndCollideGrid(&player, &vel, &map);
    
    // GROUND CHECK
    printf("%f, %f\n", prevY, vel.y);
    if (prevY > vel.y){
        isGrounded = true;
    }
    else{
        isGrounded = false;
    }
    
    // POSITION
    if(player.y > 32 * 11){
        player.y = 32 * 11;
    }
}


void GameDraw(){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    DrawTileMap();
    DrawTileGrid();
    // PLAYER
    DrawRectangle((int)player.x, (int)player.y, (int)player.w, (int)player.h, BLUE);

    EndDrawing();
    
}

void DrawTileMap(){
    for (int y = 0; y < map.h; y++){
        for (int x = 0; x < map.w; x++){
            int i = x + y * map.w;
            int tile = map.cell[i];
            if (tile){
                float cellX = (map.x + map.s * x);
                float cellY = (map.y + map.s * y);
                DrawRectangle((int)cellX, (int)cellY, map.s, map.s, LIME);
                // check tile above
                if (i - map.w >= 0 && !map.cell[i - map.w]){
                    DrawLineEx((Vector2){cellX, cellY + 3}, (Vector2){cellX + map.s, cellY + 3}, 6.0f, GREEN);
                }
            }
        }
    }
}

void DrawTileGrid(){
    Color c = (Color){255,255,255,25};
    
    for (int y = 0; y < map.h + 1; y++){
        int x1 = map.x;
        int x2 = map.x + map.w * map.s;
        int Y = map.y + map.s * y;
        DrawLine(x1, Y, x2, Y, c);
    }
    for (int x = 0; x < map.w + 1; x++){
        int y1 = map.y;
        int y2 = map.y + map.h * map.s;
        int X = map.x + map.s * x;
        DrawLine(X, y1, X, y2, c);
    }
}


