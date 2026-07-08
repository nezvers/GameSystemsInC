#ifndef SOUND_EFFECT_RAYLIB_H
#define SOUND_EFFECT_RAYLIB_H

#include "raylib.h"
#include "sound_effect.h"



// Remove name mangling for C++
#ifdef __cplusplus
extern "C" {
#endif

void SoundEffectInit(SoundEffect *sound_effect, Sound *sound);
void SoundEffectPlayRaylib(SoundEffect *sound_effect, Sound *sound, f64 time_seconds, f32 rand_f);

#ifdef __cplusplus
}
#endif

#endif // SOUND_EFFECT_RAYLIB_H

/* _________________________________________________________________ */

#ifdef SOUND_EFFECT_RAYLIB_IMPLEMENTATION
#undef SOUND_EFFECT_RAYLIB_IMPLEMENTATION

void SoundEffectInit(SoundEffect *sound_effect, Sound *sound) {
    SetSoundVolume(*sound, sound_effect->volume);
}

void SoundEffectPlayRaylib(SoundEffect *sound_effect, Sound *sound, f64 time_seconds, f32 rand_f) {
    if (!SoundEffectPlay(sound_effect, time_seconds, rand_f)) {
        return;
    }
    // Not neccessary to change volume each time
    // rl.SetSoundVolume(sound^, sound_effect.volume)
    SetSoundPitch(*sound, sound_effect->pitch);
    PlaySound(*sound);
}

#endif // SOUND_EFFECT_IMPLEMENTATION