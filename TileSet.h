#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED
#include "raylib.h"


class TileSet{
    private:
        int         collumns;   //Columns in texture
        int         rows;       //Rows in texture
        Rectangle   tile_rect;  //Used for drawing tiles
    public:
        Texture2D   texture;
        Vector2     tile_size;
        int         tile_count;      //How many tiles there are

    TileSet         (const char *textureFileName, Vector2 TileSize);//Construct
    ~TileSet        ();//Destruct
    void draw_tile  (int tileID, Vector2 position);
};


#endif // TILESET_H_INCLUDED
