#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED
#include "raylib.h"


class TileSet{
    private:
        int             collumns;                                           //Columns in texture
        int             rows;                                               //Rows in texture
        Rectangle       tile_rect;                                          //Used for drawing tiles
        Texture2D       texture;                                            //TileSet image
    public:
        Vector2         tile_size;                                          //Size of single tile
        int             tile_count;                                         //How many tiles there are

        TileSet         (const char *textureFileName, Vector2 TileSize);    //Construct
        ~TileSet        ();                                                 //Destruct
        void draw_tile  (int tileID, Vector2 position);                     //Method for draw event
};


#endif // TILESET_H_INCLUDED
