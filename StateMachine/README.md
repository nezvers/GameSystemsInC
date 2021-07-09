# State machine system in C

Super simple state machine system in C. States hold pointers to functions but they are triggered by StateMachine. You can pass data to each state function via casting it as a void pointer and cast it back to according data type.    
    
To implement .c parts need to `#define` IMPLEMENTATION guard flags before `#include` of the header file:    
```
#define NEZ_STATEMACHINE_IMPLEMENTATION
#include "sprite.h"
```

