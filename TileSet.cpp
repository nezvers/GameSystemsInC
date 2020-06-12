#include "TileSet.h"
#include <iostream>


TileSet::TileSet(const char *textureFileName, Vector2 TileSize){
    texture         = LoadTexture(textureFileName);                         //Texture2D
    tile_size       = TileSize;                                             //int
    collumns        = texture.width/int(tile_size.x);                       //int
    rows            = texture.height/int(tile_size.y);                      //int
    tile_count      = texture.width/collumns * texture.height/rows;         //int
    tile_rect       = {0.0f, 0.0f, tile_size.x, tile_size.y};    //Rectangle
}

TileSet::~TileSet(){
    UnloadTexture(texture);
}

void TileSet::draw_tile(int tileID, Vector2 position){
    if (tileID < 0){
        return;
    }
    int col = tileID % collumns;
    int row = tileID/ collumns;
    tile_rect.x = float(col * tile_size.x);
    tile_rect.y = float(row * tile_size.y);
    DrawTextureRec(texture, tile_rect, position, WHITE);
    //std::cout << "TileSetDraw: " << position.x << " " << position.y << std::endl;
}
