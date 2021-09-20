/*******************************************************************************************


********************************************************************************************/

#include "raylib.h"

#define HITBOX_COLLISIONS_IMPLEMENTATION
#include "hitbox_collision.h"

#define DEBUG

//#define PLATFORM_WEB
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

bool PLATFORMER = true; // toggle to top-down movement with TAB


void GameInit();
void GameUpdate();
void GameDraw();
void GameLoop(){GameUpdate(); GameDraw();}
void Reset();

void DrawTileGrid();
void DrawTileMap();
void DrawCoins();
void DrawPlayer();
void DrawObstacles();
void DrawScoreText();

void UpdateScreen();
void UpdatePlayer();
void UpdateCoin();


#define MAP_W 20
#define MAP_H 12
int screenWidth = 32*MAP_W;
int screenHeight = 32*MAP_H;
const int gameWidth = 32*MAP_W;
const int gameHeight = 32*MAP_H;
RenderTexture viewport;
int scale = 1;
Vector2 vpOffset = (Vector2){0.0f, 0.0f};

NezRect_f player = {32.0f * 2, 32.0f * 8, 32.0f, 32.0f};
NezRect_f obstacles[] = {
    (NezRect_f){32.0f * 15, 32.0f * 5, 32.0f, 32.0f},
};

#define COIN_COUNT 10
NezRect_f coins[COIN_COUNT] = {0};
bool visible[COIN_COUNT] = {0};
int points = 0;
int time = 0;       // For animation

NezGridMap map;
char tiles[] = {
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
    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(GameLoop, 0, 1);
    #else
    SetTargetFPS(60);
    while (!WindowShouldClose()){
        GameLoop();
    }
    UnloadRenderTexture(viewport);
    #endif
    CloseWindow();
    return 0;
}

void GameInit() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "classic game: platformer");
    viewport = LoadRenderTexture(gameWidth, gameHeight);
    map.x = 0.0f;
    map.y = 0.0f;
    map.w = MAP_W;
    map.h = MAP_H;
    map.s = 32;
    map.cell = tiles;
    Reset();
}

void Reset(){
    const float s = 32.0f;
    player = (NezRect_f){s * 2, s * 8, s, s};
    points = 0;
    time = 0;
    
    coins[0] = (NezRect_f){s * 1.5f, s * 8, 10.0f, 10.0f};
    coins[1] = (NezRect_f){s * 3.5f, s * 6, 10.0f, 10.0f};
    coins[2] = (NezRect_f){s * 4.5f, s * 6, 10.0f, 10.0f};
    coins[3] = (NezRect_f){s * 5.5f, s * 6, 10.0f, 10.0f};
    coins[4] = (NezRect_f){s * 8.5f, s * 3, 10.0f, 10.0f};
    coins[5] = (NezRect_f){s * 9.5f, s * 3, 10.0f, 10.0f};
    coins[6] = (NezRect_f){s * 10.5f, s * 3, 10.0f, 10.0f};
    coins[7] = (NezRect_f){s * 14.5f, s * 4, 10.0f, 10.0f};
    coins[8] = (NezRect_f){s * 15.5f, s * 4, 10.0f, 10.0f};
    coins[9] = (NezRect_f){s * 17.5f, s * 2, 10.0f, 10.0f};
    
    for (int i = 0; i < COIN_COUNT; i++){visible[i] = true;}
}

void GameUpdate(){
    
    UpdateScreen();// Adapt to resolution
    UpdatePlayer();
    UpdateCoin();
}

void UpdateScreen(){
    // Adapt to resolution
    if (IsWindowResized()){
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        scale = MAX(1, MIN((screenWidth/gameWidth), (screenHeight/gameHeight)));
        vpOffset.x = (screenWidth - (gameWidth * scale)) / 2;
        vpOffset.y = (screenHeight - (gameHeight * scale)) / 2;
    }
}

void UpdatePlayer(){
    
    const float maxSpd = 6.0f;
    const float acc = 0.1f;
    const float grav = 0.5f;
    const float jmpImpulse = -10.0f;
    const int jmpBufferTime = 30;
    static bool isGrounded = false;
    static int jmpBuffer = 0;
    static int dirX = 0;
    static int dirY = 0;
    static NezVec2_f vel = {0};
    
    // INPUT
    dirX = (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    dirY = (float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W));
    if(IsKeyPressed(KEY_TAB)){PLATFORMER = !PLATFORMER;}
    
    // HORIZONTAL SPEED
    vel.x += (dirX * maxSpd - vel.x) * acc;
    if (vel.x < -maxSpd){
        vel.x = -maxSpd;
    }
    else if (vel.x > maxSpd){
        vel.x = maxSpd;
    }
    
    // VERTICAL SPEED
    if (PLATFORMER){
        if (isGrounded && jmpBuffer != jmpBufferTime){
            jmpBuffer = jmpBufferTime;
        }
        if (isGrounded && IsKeyPressed(KEY_SPACE)){
            vel.y = jmpImpulse;
            jmpBuffer = 0;
        }
        else if (jmpBuffer > 0 && IsKeyPressed(KEY_SPACE)){
            vel.y = jmpImpulse;
            jmpBuffer = 0;
        }
        else{
            if (!IsKeyDown(KEY_SPACE) && vel.y < jmpImpulse * 0.2){
                vel.y = jmpImpulse * 0.2;
            }
            else{
                vel.y += grav;
                if (vel.y > -jmpImpulse){
                    vel.y = -jmpImpulse;
                }
                if (jmpBuffer > 0){
                    jmpBuffer -= 1;
                }
            }
        }
    }
    else{
        // TOP-DOWN
        vel.y += (dirY * maxSpd - vel.y) * acc;
        if (vel.y < -maxSpd){
            vel.y = -maxSpd;
        }
        else if (vel.y > maxSpd){
            vel.y = maxSpd;
        }
    }
    
    
    char flags = HitboxMoveAndCollide(&player, &vel, obstacles, 1, &map);
    //char flags = HitboxMoveAndCollideContinuous(&player, &vel, obstacles, 1, &map);
    
    
    isGrounded = flags & BOTTOM_COL;
    
    
}

void UpdateCoin(){
    for (int i = 0; i < COIN_COUNT; i++){
        if (visible[i]){
            if (HitboxCheckHitbox(&coins[i], &player)){
                visible[i] = false;
                points += 1;
            }
        }
    }
    
    if (IsKeyPressed(KEY_ENTER)){
        Reset();
    }
}

void GameDraw(){
    // Viewport scaling
    const Vector2 origin = (Vector2){0.0f, 0.0f};
    const Rectangle vp_r = (Rectangle){0.0f,gameHeight,gameWidth, -gameHeight}; // flip vertically: position = left-bottom
    Rectangle out_r = (Rectangle){vpOffset.x, vpOffset.y, gameWidth * scale, gameHeight * scale};
    
    // Render game's viewport
    BeginTextureMode(viewport);
        DrawRectangle(0, 0, gameWidth, gameHeight, SKYBLUE); // Background
        DrawTileMap();
        DrawTileGrid();
        DrawScoreText();
        DrawCoins();
        DrawObstacles();
        DrawPlayer();
    EndTextureMode();
    
    // Draw the viewport
    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(viewport.texture, vp_r, out_r, origin, 0.0f, WHITE);
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

void DrawPlayer(){
    Rectangle p = *(Rectangle*)&player;
    DrawRectangle((int)p.x, (int)p.y, (int)p.width, (int)p.height, WHITE);
    DrawRectangleLinesEx(p, 2, BLACK);
    
    // Artistic touch
    static int dirX = 0;
    dirX = (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)) * 4;
    Vector2 L1 = (Vector2){p.x + 12 + dirX, p.y + 4};
    Vector2 R1 = (Vector2){p.x + 20 + dirX, p.y + 4};
    Vector2 L2 = L1;
    L2.y += 8;
    Vector2 R2 = R1;
    R2.y += 8;
    DrawLineEx(L1, L2, 2.0f, BLACK);
    DrawLineEx(R1, R2, 2.0f, BLACK);
}

void DrawObstacles(){
    int count = sizeof(obstacles) / sizeof(obstacles[0]);
    for (int i = 0; i < count; i++){
        DrawRectangle((int)obstacles[i].x, (int)obstacles[i].y, (int)obstacles[i].w, (int)obstacles[i].h, WHITE);
    }
}

void DrawCoins(){
    time += 1;
    
    for (int i = 0; i < COIN_COUNT; i++){
        if (visible[i]){
            Rectangle c = *(Rectangle*)&coins[i];
            float y = (float)sin(2 * PI * (time / 60.0f * 0.5) + (c.x * 5)) * 4; // pseudo random offset floating
            float x = (float)sin(2 * PI * (time / 60.0f * 2)) * 4;
            DrawRectangle((int)(c.x + 4 + x * 0.5), (int)(c.y + y), (int)(c.width - 4 - x), (int)c.height, GOLD);
        }
    }
}

void DrawScoreText(){
    const char *text;
    if (points == COIN_COUNT){
        text = TextFormat("Pres 'ENTER' to restart!");
    }
    else{
        text = TextFormat("Score: %d", points);
    }
    
    const int size = 24;
    int x = gameWidth /2 - MeasureText(text, size) / 2;
    int y = 48;
    
    DrawText(text, x, y+1, size, BLACK);
    DrawText(text, x, y, size, WHITE);
    
}

