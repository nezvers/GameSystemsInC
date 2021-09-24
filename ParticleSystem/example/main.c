/*******************************************************************************************


********************************************************************************************/
#include "raylib.h"

#define PARTICLE_SYSTEM_IMPLEMENTATION
#include "particle_system.h"


//#define PLATFORM_WEB
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void GameInit();
void GameUpdate();
void GameDraw();
void GameLoop(){GameUpdate(); GameDraw();}
void Reset();

void DrawParticles();

void UpdateScreen();
void UpdateParticles();



int screenWidth = 640;
int screenHeight = 360;
const int gameWidth = 320;
const int gameHeight = 180;
RenderTexture viewport;
int scale = 2;
Vector2 vpOffset = (Vector2){0.0f, 0.0f};
int time = 0;       // For animation



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
}


void GameUpdate(){
    UpdateScreen();// Adapt to resolution
    UpdateParticles();
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

void UpdateParticles(){
    
}


void GameDraw(){
    // Viewport scaling
    const Vector2 origin = (Vector2){0.0f, 0.0f};
    const Rectangle vp_r = (Rectangle){0.0f,gameHeight,gameWidth, -gameHeight}; // flip vertically: position = left-bottom
    Rectangle out_r = (Rectangle){vpOffset.x, vpOffset.y, gameWidth * scale, gameHeight * scale};
    
    // Render game's viewport
    BeginTextureMode(viewport);
        DrawParticles();
    EndTextureMode();
    
    // Draw the viewport
    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(viewport.texture, vp_r, out_r, origin, 0.0f, WHITE);
    EndDrawing();
}

void DrawParticles(){
    
}
