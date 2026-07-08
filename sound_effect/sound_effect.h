#ifndef SOUND_EFFECT_H
#define SOUND_EFFECT_H

#include <stdint.h>

typedef uint32_t u32;
typedef int32_t  i32;
typedef float    f32;
typedef double   f64;

typedef struct {
    f32 volume;
    f32 pitch_rand_min;
    f32 pitch_rand_max;
    f32 pitch_min;
    f32 pitch_max;
    f32 pitch_increment;    // For fast repeated trigger
    f32 retrigger_treshold; // How soon can be triggered again
    f32 retrigger_interval; // If triggered in this time again, a pitch is added
    f32 pitch_return;       // Time to return to original pitch
    // Calculated at trigger
    f32 pitch;              // Current pitch
    f64 last_time;          // Keep track of trigger time
} SoundEffect;

// Remove name mangling for C++
#ifdef __cplusplus
extern "C" {
#endif

bool SoundEffectPlay(SoundEffect *sound, f64 time_seconds, f32 rand_f);

#ifdef __cplusplus
}
#endif

#endif // SOUND_EFFECT_H

/* _________________________________________________________________ */

#ifdef SOUND_EFFECT_IMPLEMENTATION
#undef SOUND_EFFECT_IMPLEMENTATION

static inline f32 SoundEffectLerp(f32 a, f32 b, f32 t) {
    return a + (b - a) * t;
}

static void SoundEffectUpdatePitch(SoundEffect *sound, f32 delta, f32 rand_f) {
    if (delta < sound->retrigger_interval) {
        if (sound->pitch > sound->pitch_max) {
            sound->pitch = sound->pitch_max + (SoundEffectLerp(sound->pitch_rand_min, sound->pitch_rand_max, rand_f) - 1);
        }
        else if (sound->pitch < sound->pitch_min) {
            sound->pitch = sound->pitch_min + (SoundEffectLerp(sound->pitch_rand_min, sound->pitch_rand_max, rand_f) - 1);
        }
    }
    else if (delta < sound->retrigger_interval + sound->pitch_return) {
        f32 pitch_default = SoundEffectLerp(sound->pitch_rand_min, sound->pitch_rand_max, 0.5f);
        f32 t = (delta - sound->retrigger_interval) / sound->pitch_return;
        sound->pitch = SoundEffectLerp(sound->pitch, pitch_default, t);
    } else {
        sound->pitch = SoundEffectLerp(sound->pitch_rand_min, sound->pitch_rand_max, rand_f);
    }
}

bool SoundEffectPlay(SoundEffect *sound, f64 time_seconds, f32 rand_f) {
    if (time_seconds < sound->last_time + sound->retrigger_treshold) {
        return false;
    }
    f32 delta = (time_seconds - sound->last_time);
    sound->last_time = time_seconds;
    // GetPitch
    
    return true;
}

#endif // SOUND_EFFECT_IMPLEMENTATION