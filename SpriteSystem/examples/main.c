/*******************************************************************************************
*
*   
*
********************************************************************************************/

#include "raylib.h"

#define NEZ_SPRITE_IMPLEMENTATION
#include "sprite.h"

//#define PLATFORM_WEB

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif


int screenWidth = 640;
int screenHeight = 360;

void InitGame(void);
void GameLoop(void);

void UpdateButtons(void);
void DrawInterface(void);

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

Rectangle xOrigPlus;
Rectangle xOrigMinus;
Rectangle yOrigPlus;
Rectangle yOrigMinus;

Rectangle xScalePlus;
Rectangle xScaleMinus;
Rectangle yScalePlus;
Rectangle yScaleMinus;


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
    UpdateButtons();
	
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
        UpdateButtons();
	}
    UpdateInput();
	SpritePlay(sprite, currentAnimation, 1.0/60.0);
	
	BeginDrawing();

        ClearBackground(RAYWHITE);
		BeginMode2D(camera);
		
		SpriteDraw(sprite);
		DrawLine(sprite->x -2, sprite->y, sprite->x +2, sprite->y,BLACK);
		DrawLine(sprite->x, sprite->y -2, sprite->x, sprite->y +2,BLACK);
        
        
		EndMode2D();
        
        DrawInterface();
        
    EndDrawing();
}

void UpdateInput(){
	if(IsKeyPressed(KEY_RIGHT)){
		sprite->xScale = 1.0f;
		printf("xScale: %f\n", sprite->xScale);
	}
	if(IsKeyPressed(KEY_LEFT)){
		sprite->xScale = -1.0f;
		printf("xScale: %f\n", sprite->xScale);
	}
	if(IsKeyPressed(KEY_UP)){
		sprite->yScale = 1.0f;
		printf("yScale: %f\n", sprite->yScale);
	}
	if(IsKeyPressed(KEY_DOWN)){
		sprite->yScale = -1.0f;
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
    
    // MOUSE TO INTERFACE
    if(IsMouseButtonPressed(0)){
        Vector2 mouse = GetMousePosition();
        if(CheckCollisionPointRec(mouse, xOrigPlus)){
            sprite->xOrigin++;
        }else if(CheckCollisionPointRec(mouse, xOrigMinus)){
            sprite->xOrigin--;
        }else if(CheckCollisionPointRec(mouse, yOrigPlus)){
            sprite->yOrigin++;
        }else if(CheckCollisionPointRec(mouse, yOrigMinus)){
            sprite->yOrigin--;
        }else if(CheckCollisionPointRec(mouse, xScalePlus)){
            sprite->xScale+= 0.1;
        }else if(CheckCollisionPointRec(mouse, xScaleMinus)){
            sprite->xScale-= 0.1;
        }else if(CheckCollisionPointRec(mouse, yScalePlus)){
            sprite->yScale+= 0.1;
        }else if(CheckCollisionPointRec(mouse, yScaleMinus)){
            sprite->yScale-= 0.1;
        }
    }
}

void UpdateButtons(){
    float w = 16.0f;
    float h = 16.0f;
    xOrigPlus = (Rectangle){screenWidth-(w*2),      0.0f, w, h};
    xOrigMinus = (Rectangle){screenWidth-(w*3) -2,  0.0f, w, h};
    yOrigPlus = (Rectangle){screenWidth-(w*2),      h +2, w, h};
    yOrigMinus = (Rectangle){screenWidth-(w*3) -2,  h +2, w, h};
    
    xScalePlus = (Rectangle){screenWidth-(w*2),     h*3 +6, w, h};
    xScaleMinus = (Rectangle){screenWidth-(w*3) -2, h*3 +6, w, h};
    yScalePlus = (Rectangle){screenWidth-(w*2),     h*4 +8, w, h};
    yScaleMinus = (Rectangle){screenWidth-(w*3) -2, h*4 +8, w, h};
}

void DrawInterface(void){
    //Origin
    DrawRectangleLinesEx(xOrigPlus, 1, LIGHTGRAY);
    DrawRectangleLinesEx(xOrigMinus, 1, LIGHTGRAY);
    DrawRectangleLinesEx(yOrigPlus, 1, LIGHTGRAY);
    DrawRectangleLinesEx(yOrigMinus, 1, LIGHTGRAY);
    DrawText("x+", (int)xOrigPlus.x+4, (int)xOrigPlus.y+4, 8, BLACK);
    DrawText("x-", (int)xOrigMinus.x+4, (int)xOrigMinus.y+4, 8, BLACK);
    DrawText("y+", (int)yOrigPlus.x+4, (int)yOrigPlus.y+4, 8, BLACK);
    DrawText("y-", (int)yOrigMinus.x+4, (int)yOrigMinus.y+4, 8, BLACK);
    
    const char * text = TextFormat("origin: (%d, %d)", sprite->xOrigin, sprite->yOrigin);
    int textWidth = MeasureText(text, 8);
    DrawText(text, screenWidth-textWidth - (int)yOrigMinus.width, (int)yOrigMinus.y+(int)yOrigMinus.height +2, 8, BLACK);
    
    // Scale
    DrawRectangleLinesEx(xScalePlus, 1, LIGHTGRAY);
    DrawRectangleLinesEx(xScaleMinus, 1, LIGHTGRAY);
    DrawRectangleLinesEx(yScalePlus, 1, LIGHTGRAY);
    DrawRectangleLinesEx(yScaleMinus, 1, LIGHTGRAY);
    DrawText("x+", (int)xScalePlus.x+4, (int)xScalePlus.y+4, 8, BLACK);
    DrawText("x-", (int)xScaleMinus.x+4, (int)xScaleMinus.y+4, 8, BLACK);
    DrawText("y+", (int)yScalePlus.x+4, (int)yScalePlus.y+4, 8, BLACK);
    DrawText("y-", (int)yScaleMinus.x+4, (int)yScaleMinus.y+4, 8, BLACK);
    
    const char * text2 = TextFormat("scale: (%.2f, %.2f)", sprite->xScale, sprite->yScale);
    textWidth = MeasureText(text2, 8);
    DrawText(text2, screenWidth-textWidth - (int)yScaleMinus.width, (int)yScaleMinus.y+(int)yScaleMinus.height +2, 8, BLACK);
}





