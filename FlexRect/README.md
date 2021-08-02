# FlexRect (in progress)
## A single header library for flexible rectangles that are usable for GUI positioning.

To use function implementation `#define NEZ_FLEXRECT_IMPLEMENTATION` for library's .c part.    
Library allows to create rectangles that positions and sizes depending on anchor and margins from given or parent rectangle and includes minimal width & height.    
FlexRect are the resizing rectangles and FlexRect2 are plain {x, y, w, h} struct. FlexRect.r holds it's rectangle area of FlexRect2.    
    
![](https://raw.githubusercontent.com/nezvers/GameSystemsInC/master/FlexRect/example/FlexRectPreview.gif)    
