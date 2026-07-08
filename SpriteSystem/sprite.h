#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

typedef uint32_t u32;
typedef int32_t  i32;
typedef float    f32;

typedef struct {
    f32 x;
    f32 y;
} vec2;

typedef struct {
    f32 x;
    f32 y;
    f32 w;
    f32 h;
} rectf;

// Animation frame list, asuming all frames have same region size
typedef struct {
    vec2 *data; // Frame position array
    u32 count;
    vec2 size; // width & height
} Frames;

typedef struct {
    Frames **frames; // Array of Frame pointers
    u32 count;
    u32 animation_index;
    u32 image_index;
    f32 frame_rate;
    f32 time;
} AnimationSet;


typedef struct {
    AnimationSet animation_set;
    vec2 position;
    vec2 origin;
    vec2 offset;
    vec2 scale;
    f32 rotation;
} Sprite;

/* _________________________________________________________________ */

// Remove name mangling for C++
#ifdef __cplusplus
extern "C" {
#endif

void ChangeAnimation(AnimationSet *animation_set, u32 new_animation);
void UpdateAnimation(AnimationSet *animation_set, f32 delta_time);
void UpdateSprite(Sprite *sprite, f32 delta_time);
rectf GetAnimationFrame(AnimationSet *animation_set);
void GetSpriteFrame(Sprite *sprite, rectf *sprite_rect, rectf *texture_rect);

#ifdef __cplusplus
}
#endif

#endif // SPRITE_H


/* _________________________________________________________________ */

#ifdef SPRITE_IMPLEMENTATION
#undef SPRITE_IMPLEMENTATION

#include <assert.h>

void ChangeAnimation(AnimationSet *animation_set, u32 new_animation) {
    assert(new_animation < animation_set->count);
    animation_set->animation_index = new_animation;
    animation_set->image_index = 0;
    animation_set->time = 0;
}

void UpdateAnimation(AnimationSet *animation_set, f32 delta_time) {
    animation_set->time += delta_time * animation_set->frame_rate;
    if (animation_set->time < 1) { return; }

    const Frames *frame = animation_set->frames[animation_set->animation_index];
    const u32 image_count = frame->count;
    const u32 increment = animation_set->time;
    animation_set->time -= increment;
    animation_set->image_index = (animation_set->image_index + increment) % image_count;
}

void UpdateSprite(Sprite *sprite, f32 delta_time) {
    UpdateAnimation(&sprite->animation_set, delta_time);
}

rectf GetAnimationFrame(AnimationSet *animation_set) {
    const Frames *frame = animation_set->frames[animation_set->animation_index];
    const vec2 pos = frame->data[animation_set->image_index];
    const vec2 size = frame->size;
    const rectf result = {pos.x, pos.y, size.x, size.y};
    return result;
}

// texture_out = source rectangle from texture
// sprite_out = destination rectangle on screen
void GetSpriteFrame(Sprite *sprite, rectf *texture_out, rectf *sprite_out) {
    *texture_out = GetAnimationFrame(&sprite->animation_set);
    *sprite_out = (rectf){sprite->position.x, sprite->position.y, texture_out->w, texture_out->h};
}

#endif