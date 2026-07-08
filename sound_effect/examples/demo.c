

#include "raylib.h"

#define SOUND_EFFECT_IMPLEMENTATION
#include "sound_effect.h"

#define SOUND_EFFECT_RAYLIB_IMPLEMENTATION
#include "sound_effect_raylib.h"

Sound button_sound;
Sound damage_sound;

SoundEffect button_sfx = {
    .volume = 1.f,
    .pitch_rand_min = 0.9f,
    .pitch_rand_max = 1.2f,
    .pitch_min = 0.5f,
    .pitch_max = 1.75f,
    .pitch_increment = 0.01f,     // Pitch change on fast retrigger
    .retrigger_treshold = 0.02f,  // Don't play if sooner than this
    .retrigger_interval = 0.5f,   // Applay pitch_increment
    .pitch_return = 1,
};

SoundEffect damage_sfx = {
    .volume = 0.5f,
    .pitch_rand_min = 0.9f,
    .pitch_rand_max = 1.2f,
    .pitch_min = 0.5f,
    .pitch_max = 1.75f,
    .pitch_increment = 0.01f,
    .retrigger_treshold = 0.02f,
    .retrigger_interval = 0.5f,
    .pitch_return = 1,
};

void init() {
    InitAudioDevice();
    button_sound = LoadSound(ASSETS_PATH "/sounds/button_sound.wav");
    damage_sound = LoadSound(ASSETS_PATH "/sounds/damage_sound.wav");
}

void finit() {
    UnloadSound(button_sound);
    UnloadSound(damage_sound);
    CloseAudioDevice();
}

void update() {
    f32 current_time = GetTime();
    f32 rand_f = (GetRandomValue(0, 1000000) / 1000000.f) / 1.0f;

    if (IsKeyPressed(KEY_SPACE)) {
        SoundEffectPlayRaylib(&button_sfx, &button_sound, current_time, rand_f);
    }

    if (IsKeyPressed(KEY_ENTER)) {
        SoundEffectPlayRaylib(&damage_sfx, &damage_sound, current_time, rand_f);
    }
}

void draw() {
    DrawText("Space = button sound", 10, 10, 30, BLACK);
    DrawText("Enter = damage sound", 10, 50, 30, BLACK);
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);

    init();

    // Main game loop
    while (!WindowShouldClose())
    {
        update();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw();

        EndDrawing();
    }

    finit();
    CloseWindow();        // Close window and OpenGL context

    return 0;
}