# Sprite system in C

![](https://github.com/nezvers/GameSystemsInC/raw/master/SpriteSystem/examples/resources/Preview_Sprite_anim_scale.gif)

## sprite_rect.h
- Framework independent
- No allocations, user pre-allocated/ managed memory
- Overridable Vector2 & Rectangle typedefs
```C
#define NEZ_SPRITE_RECT_IMPLEMENTATION // use for *.c section implementation
#define NEZ_VEC2_F Vector2
#define NEZ_RECT_F Rectangle
#include "sprite_rect.h"
```

## sprite.h
- Uses allocation
- Tied to Raylib's Vector2, Rectangle, Texture
    
```
#define NEZ_SPRITE_IMPLEMENTATION // use for *.c section
#include "sprite.h"
```

