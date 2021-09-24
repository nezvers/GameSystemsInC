#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
#include "stdio.h"
#include "stdbool.h"

#ifndef NEZVEC2_F
#define NEZVEC2_F
typedef struct {
    float x;
    float y;
} NezVec2_f;
#endif // NEZVEC2_F


// For user to include in custom struct header (C style inheritance)
typedef struct Particle{
	float lifetime; // time left from 1,0 to 0.0
	float delta; // value to multiply games delta affecting lifetime
	NezVec2_f pos; // position of the particle
}Particle;

typedef struct{
	int bufferSize; // how many particles alocated for the emiter
	int emisionCount; // how many in burst or over 1sec
	bool burst; // release particles in one-shot or over time
	float width;
	float height;
	float minLifetime;
	float maxLifetime;
	size_t sizeofParticle; // particle struct need to be expanded for use case
	void (*init)(Particle*); // callback for particle initialization
	void (*update)(Particle*); // callback for particle update
	void (*draw)(Particle*); // callback for particle drawing
}EmiterConfig;

typedef struct{
	bool emitting;
	int aliveCount;
	EmiterConfig config;
	void *particles;
}Emitter;

#ifndef NEZPSAPI
    #ifdef NEZ_PARTICLES_STATIC
        #define NEZPSAPI static
    #else
        #define NEZPSAPI extern
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif //PARTICLE_SYSTEM_H

#ifdef PARTICLE_SYSTEM_IMPLEMENTATION
#undef PARTICLE_SYSTEM_IMPLEMENTATION

#endif //PARTICLE_SYSTEM_IMPLEMENTATION