#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED
#include "raylib.h"


class TileSet{
    private:
        int         tiles;
        int         collumns;
        int         rows;
        Rectangle   tile_rect;
    public:
        Texture2D   texture;
        Vector2     tile_size;

    TileSet         (const char *textureFileName, Vector2 TileSize);//Construct
    ~TileSet        ();//Destruct
    void draw_tile  (int tileID, Vector2 position);
};


#endif // TILESET_H_INCLUDED
