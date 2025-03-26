/*******************************************************************************************
*
*
*
********************************************************************************************/

#include "raylib.h"

#define NEZ_FLEXRECT_IMPLEMENTATION
#include "flexrect.h"


int screenWidth = 640;
int screenHeight = 360;

void InitGame(void);
void GameLoop(void);

NezRect_i rect;

FlexRect* flexRect;
FlexRect* flexChild1;

void Resize();

int main() {
	InitGame();

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		GameLoop();
	}


	CloseWindow();
	return 0;
}


void InitGame() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "FlexRect test");

	rect = (NezRect_i){ 0, 0, screenWidth, screenHeight };
	flexRect = FlexRectNew(&rect, 0.0f, 0.0f, 1.0f, 1.0f, 10, 10, 10, 10, 0, 0);
	flexChild1 = FlexRectCreateChild(flexRect, 0.0f, 0.0f, 1.0f, 0.5f, 10, 10, 10, 0, 300, 100);
}

void GameLoop(void) {
	if (IsWindowResized()) {
		screenWidth = GetScreenWidth();
		screenHeight = GetScreenHeight();
		Resize();
	}


	BeginDrawing();

	ClearBackground(RAYWHITE);

	if (flexRect) {
		DrawRectangleLines(flexRect->r.x, flexRect->r.y, flexRect->r.w, flexRect->r.h, RED);
		DrawRectangleLines(flexChild1->r.x, flexChild1->r.y, flexChild1->r.w, flexChild1->r.h, RED);
	}

	EndDrawing();
}


void Resize() {
	rect = (NezRect_i){ 0, 0, screenWidth, screenHeight };
	FlexRectResize(flexRect, &rect);
}

