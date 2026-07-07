# Sprite system in C



## sprit.h
- Manages rectangle from texture to drawing rectangle
- Framework independent `sprite.h`
- Glue code for Raylib `sprite_raylib.h`
- No allocations, user pre-allocated/ managed memory
```C
#define SPRITE_IMPLEMENTATION // use for *.c section implementation
#include "sprite_rect.h"
```

## sprite.h
- Uses allocation
- Tied to Raylib's Vector2, Rectangle, Texture
    
```
#define NEZ_SPRITE_IMPLEMENTATION // use for *.c section
#include "sprite.h"
```

