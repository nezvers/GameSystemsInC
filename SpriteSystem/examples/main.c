/*******************************************************************************************
*
*   
*
********************************************************************************************/

#include "raylib.h"

#define NEZ_SPRITE_IMPLEMENTATION
#include "../sprite.h"

//#define PLATFORM_WEB

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif


int screenWidth = 320;
int screenHeight = 180;

void InitGame(void);
void GameLoop(void);

Texture texture;
Camera2D camera = {(Vector2){0}, (Vector2){0}, 0.0f, 4.0f};
float x;
float y;
Sprite *sprite;
SpriteAnimation idleAnim;
SpriteAnimation walkAnim;
int idle;
int walk;
int currentAnimation;

//----------------------------------------------------------------------------------
int main(){
    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(GameLoop, 0, 1);
#else
    SetTargetFPS(60);
    
	while (!WindowShouldClose()){
        GameLoop();
    }
#endif
	
	UnloadTexture(texture); 
	
    CloseWindow();
    return 0;
}


void InitGame(){
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "SpriteSystem test");
	
	texture = LoadTexture("../resources/player_sheet.png");
	
	sprite = SpriteNewFromSheet(&texture, 16, 16, 8, 16);
	sprite->x = 30;
	sprite->y = 30;
	
	int idleList[] = {0};
	idleAnim = SpriteAnimationNew(idleList, 1, 8);
	idle = SpriteAddAnimation(sprite, idleAnim);
	
	int walkList[] = {1,2,3,4,5,6};
	walkAnim = SpriteAnimationNew(walkList, 6, 8);
	walk = SpriteAddAnimation(sprite, walkAnim);
	
	x = (float)sprite->x;
	y = (float)sprite->y;
}

void UpdateInput();
void GameLoop(void){
	if (IsWindowResized()){
		screenWidth = GetScreenWidth();
		screenHeight = GetScreenHeight();
	}
    UpdateInput();
	SpritePlay(sprite, currentAnimation, 1.0/60.0);
	
	BeginDrawing();

        ClearBackground(RAYWHITE);
		BeginMode2D(camera);
		
		SpriteDraw(sprite);
		
		EndMode2D();
    EndDrawing();
}

void UpdateInput(){
	if(IsKeyPressed(KEY_RIGHT)){
		sprite->xScale += 0.1;
		printf("xScale: %f\n", sprite->xScale);
	}
	if(IsKeyPressed(KEY_LEFT)){
		sprite->xScale -= 0.1;
		printf("xScale: %f\n", sprite->xScale);
	}
	if(IsKeyPressed(KEY_UP)){
		sprite->yScale += 0.1;
		printf("yScale: %f\n", sprite->yScale);
	}
	if(IsKeyPressed(KEY_DOWN)){
		sprite->yScale -= 0.1;
		printf("yScale: %f\n", sprite->yScale);
	}
	
	int xDir = (int)IsKeyDown(KEY_D) - (int)IsKeyDown(KEY_A);
	int yDir = (int)IsKeyDown(KEY_S) - (int)IsKeyDown(KEY_W);
    float spd = 0.4f;
    x += spd * xDir;
    y += spd *yDir;
    sprite->x = (int)x;
    sprite->y = (int)y;
    
	if(xDir != 0 || yDir != 0){
		currentAnimation = walk;
	}
    else{
        currentAnimation = idle;
    }
    
    if (xDir != 0){
        sprite->xScale = xDir;
    }
}


