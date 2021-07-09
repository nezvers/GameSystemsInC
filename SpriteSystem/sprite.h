#ifndef NEZ_SPRITE_H
#define NEZ_SPRITE_H

#include "raylib.h"
#include "stdlib.h"
#include <stdio.h>

typedef struct{
	float image;        // used as timer (moduled to imageCount) and on drawing floored(cast as int)
	int imageCount;     // number of images in the animation
	int fps;            // time in seconds for each frame
	int *imageID;       // array of sprite IDs
}SpriteAnimation;

typedef struct{
	int x;
	int y;
	int w;                  // individual sprite image width
	int h;                  // individual sprite image height
	int xOrigin;		// origin x point against source rectangle
	int yOrigin;		// origin y point against source rectangle
	float xScale;		// scale output rectangle width depending on origin x position
	float yScale;		// scale output rectangle height depending on origin y position
	int imageCount;		// number of all frames
	int animation;		// ID for current animation
	int animationCount;	// animationList size
	Texture2D *texture;	// texture holding pointer to sprite sheet texture
	Vector2 *imagePos;	// list of source x&y on the texture
	SpriteAnimation *animationList;
}Sprite;

#ifndef NEZSPAPI
    #ifdef NEZ_SPRITE_STATIC
        #define NEZSSAPI static
    #else
        #define NEZSSAPI extern
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Automatically create sprite image collection
NEZSSAPI Sprite*
SpriteNewFromSheet(Texture2D *tex, int w, int h, int xO, int yO);
// Version for predetermined frame position list array
NEZSSAPI Sprite*
SpriteNewFromAtlas(Texture2D *tex, int w, int h, int xO, int yO, Vector2 *posList, int posCount);
// Free allocated memory
NEZSSAPI void
SpriteDestroy(Sprite *sprite, bool destroyAnimations);
// Adds SpriteAnimation to the Sprite and returns animation ID for it
NEZSSAPI int
SpriteAddAnimation(Sprite *sprite, SpriteAnimation anim);
// Adds SpriteAnimation list and returns ID for first anamation in the new list
NEZSSAPI int
SpriteAddAnimationList(Sprite *sprite, SpriteAnimation *anim, int animationCount);
// Create animation from list of sprite image collection
NEZSSAPI SpriteAnimation
SpriteAnimationNew(int *imageIDList, int imageCount,  int fps);
// Free memory for animation
NEZSSAPI void
SpriteAnimationDestroy(SpriteAnimation *anim);
// Animation update, requires delta time (seconds) since last frame. Or (1.0/60.0) for constant time of 60FPS
NEZSSAPI void
SpritePlay(Sprite *sprite, int animID, float delta);
// Animation update that can trigger callback for AnimationFinished with animation ID
NEZSSAPI void
SpritePlayWithCallback(Sprite *sprite, int animID, float delta, void (*AnimationFinished)(int));
// Just draw the sprite
NEZSSAPI void
SpriteDraw(Sprite *sprite);

#ifdef __cplusplus
}
#endif
#endif // NEZ_SPRITE_H

//----------------------------------------------------------------------------

#ifdef NEZ_SPRITE_IMPLEMENTATION
#undef NEZ_SPRITE_IMPLEMENTATION
float SpriteAbs(float x){return x>0.0 ? x : -x;}

Sprite*
SpriteNewFromSheet(Texture2D *tex, int w, int h, int xO, int yO){
	Sprite *s = (Sprite*)malloc(sizeof(Sprite));
	*s = (Sprite){0};
	s->w = w;
	s->h = h;
	s->xOrigin = xO;
	s->yOrigin = yO;
	s->xScale = 1.0f;
	s->yScale = 1.0f;
	s->texture = tex;
	s->animation = -1;
	
	int xFrames = tex->width / w;
	int yFrames = tex->height / h;
	s->imageCount = xFrames *yFrames;
	s->imagePos = (Vector2*)malloc(sizeof(Vector2)*xFrames *yFrames);
	
	for(int y=0; y<yFrames; y++){
		for(int x=0; x<xFrames; x++){
			s->imagePos[y *xFrames +x] = (Vector2){(float)x*w, (float)y*h};
		}
	}
	
	return s;
}

Sprite*
SpriteNewFromAtlas(Texture2D *tex, int w, int h, int xO, int yO, Vector2 *posList, int posCount){
	Sprite *s = (Sprite*)malloc(sizeof(Sprite));
	*s = (Sprite){0};
	s->w = w;
	s->h = h;
	s->xOrigin = xO;
	s->yOrigin = yO;
	s->xScale = 1.0f;
	s->yScale = 1.0f;
	s->texture = tex;
	s->animation = -1;
	
	s->imageCount = posCount;
	s->imagePos = (Vector2*)malloc(sizeof(Vector2) *posCount);
	
	for(int i=0; i<posCount; i++){
		s->imagePos[i] = posList[i];
	}
	
	return s;
}

void
SpriteDestroy(Sprite *sprite, bool destroyAnimations){
	if(destroyAnimations){
		for(int i=0; i<sprite->animationCount; i++){
			SpriteAnimationDestroy(&sprite->animationList[i]);
		}
	}
	free(sprite->animationList);
	free(sprite->imagePos);
	free(sprite);
}

int
SpriteAddAnimation(Sprite *sprite, SpriteAnimation anim){
	if(!anim.imageID){
        printf("Invalid animation\n");
        return -1;
    }
    
    SpriteAnimation *newList = (SpriteAnimation*)malloc(sizeof(SpriteAnimation) *(sprite->animationCount +1));
	for(int i=0; i< sprite->animationCount; i++){
		newList[i] = sprite->animationList[i];
	}
	free(sprite->animationList);
	sprite->animationList = newList;
	sprite->animationList[sprite->animationCount] = anim;
	sprite->animationCount++;
	return sprite->animationCount -1;
}

int
SpriteAddAnimationList(Sprite *sprite, SpriteAnimation *anim, int animationCount){
	SpriteAnimation *newList = (SpriteAnimation*)malloc(sizeof(SpriteAnimation) *(sprite->animationCount +animationCount));
	//old animations
	for(int i=0; i< sprite->animationCount; i++){
		newList[i] = sprite->animationList[i];
	}
	//new animations
	for(int i=0; i< animationCount; i++){
		newList[sprite->animationCount +i] = anim[i];
	}
	free(sprite->animationList);
	sprite->animationList = newList;
	sprite->animationCount += animationCount;
	return sprite->animationCount -animationCount;
}



SpriteAnimation
SpriteAnimationNew(int *imageIDList, int imageCount,  int fps){
	SpriteAnimation sa = {0};
	sa.fps = fps;
	sa.imageCount = imageCount;
	sa.imageID = (int*)malloc(sizeof(int) *imageCount);
	for (int i=0; i<imageCount; i++){
		sa.imageID[i] = imageIDList[i];
	}
	return sa;
}

void
SpriteAnimationDestroy(SpriteAnimation *anim){
	free(anim->imageID);
}


void
SpritePlay(Sprite *sprite, int animID, float delta){
	if(sprite->animation != animID){
		sprite->animation = animID;
		sprite->animationList[animID].image = 0.0f;
	}
	else{
		sprite->animationList[animID].image += delta *sprite->animationList[animID].fps;
		if(sprite->animationList[animID].image > sprite->animationList[animID].imageCount){
			sprite->animationList[animID].image -= (float)(int)sprite->animationList[animID].image;
		}
	}
}

void
SpritePlayWithCallback(Sprite *sprite, int animID, float delta, void (*AnimationFinished)(int)){
	if(sprite->animation != animID){
		sprite->animation = animID;
		sprite->animationList[animID].image = 0.0f;
	}
	else{
		sprite->animationList[animID].image += delta *sprite->animationList[animID].fps;
		if(sprite->animationList[animID].image > sprite->animationList[animID].imageCount){
			sprite->animationList[animID].image -= (float)(int)sprite->animationList[animID].image;
			AnimationFinished(animID);
		}
	}
}


void
SpriteDraw(Sprite *sprite){
	SpriteAnimation *anim = &sprite->animationList[sprite->animation];
	int id = anim->imageID[(int)anim->image];
	Vector2 sourcePos = sprite->imagePos[id];
	
	Vector2 offset = (Vector2){sprite->xOrigin *sprite->xScale, sprite->yOrigin *sprite->yScale};
    
	
    float x = sprite->x -offset.x;
    float y = sprite->y -offset.y;
    float w = sprite->w *SpriteAbs(sprite->xScale);
    float h = sprite->h *SpriteAbs(sprite->yScale);
    
    
	Rectangle source = {sourcePos.x, sourcePos.y, (float)sprite->w, (float)sprite->h};
    if (sprite->xScale < 0.0){
        source.width = -(float)sprite->w;
		x -= w;
    }
    if (sprite->yScale < 0.0){
        source.y += sprite->h;
        source.height *= -1;
		y -= h;
    }
	
    Rectangle dest = {x, y, w, h};
    Vector2 origin = (Vector2){ 0.0f,  0.0f};
    float rotation = 0.0f;
    
	DrawTexturePro(*sprite->texture, source, dest, origin, rotation, WHITE);
}

#endif // NEZ_SPRITE_IMPLEMENTATION


