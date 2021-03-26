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


int screenWidth = 640;
int screenHeight = 360;

void InitGame(void);
void GameLoop(void);

Texture texture;
Camera2D camera = {(Vector2){0}, (Vector2){0}, 0.0f, 2.0f};
float x;
float y;
Sprite *sprite;
SpriteAnimation anim;
int walk;

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
	
	int list[] = {1,2,3,4,5,6};
	anim = SpriteAnimationNew(5, list, 6);
	
	walk = SpriteAddAnimation(sprite, anim);
	
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
	SpritePlay(sprite, walk, 1.0/60.0);
	
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
	
	
	if(IsKeyDown(KEY_D)){
		x += 0.2;
		sprite->x = (int)x;
	}
	if(IsKeyDown(KEY_A)){
		x -= 0.2;
		sprite->x = (int)x;
	}
	if(IsKeyDown(KEY_S)){
		y += 0.2;
		sprite->y = (int)y;
	}
	if(IsKeyDown(KEY_W)){
		y -= 0.2;
		sprite->y = (int)y;
	}
}


