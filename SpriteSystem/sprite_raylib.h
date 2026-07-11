#ifndef SPRITE_RAYLIB_H
#define SPRITE_RAYLIB_H

#include "raylib.h"
#include "sprite.h"

typedef struct {
    Sprite sprite;
    Texture2D *texture;
    Color tint;
} SpriteRaylib;

// Remove name mangling for C++
#ifdef __cplusplus
extern "C" {
#endif

void DrawSpriteRaylib(SpriteRaylib *raylib_sprite);
void DrawSprite(Sprite *sprite, Texture2D *texture, Color tint);

#ifdef __cplusplus
}
#endif

#endif // SPRITE_RAYLIB_H


#ifdef SPRITE_RAYLIB_IMPLEMENTATION
#undef SPRITE_RAYLIB_IMPLEMENTATION

#include <math.h>

void DrawSpriteRaylib(SpriteRaylib *raylib_sprite) {
    DrawSprite(&raylib_sprite->sprite, raylib_sprite->texture, raylib_sprite->tint);
}

void DrawSprite(Sprite *sprite, Texture2D *texture, Color tint) {
    rectf source_rect;
    rectf target_rect;
    GetSpriteFrame(sprite, &source_rect, &target_rect);
    target_rect.x += sprite->offset.x;
    target_rect.y += sprite->offset.y;

    vec2 origin = {sprite->origin.x * abs(sprite->scale.x), sprite->origin.y * abs(sprite->scale.y)};

    if (sprite->scale.x < 0.f) {
        source_rect.w *= -1;
    }

    if (sprite->scale.y < 0.f) {
        origin.y = sprite->origin.y * sprite->scale.y - target_rect.h * sprite->scale.y;
        source_rect.h *= -1;
    }
    target_rect.w *= sprite->scale.x;
    target_rect.h *= sprite->scale.y;

    DrawTexturePro(
        *texture,
        *(Rectangle*)& source_rect,
        *(Rectangle*)& target_rect,
        *(Vector2*)& origin,
        sprite->rotation,
        tint
    );
}

#endif // SPRITE_RAYLIB_IMPLEMENTATION