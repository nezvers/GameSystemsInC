#include "TileMap.h"
#include "raylib.h"
#include <cmath>
#include <iostream>


TileMap::TileMap(TileSet& newTileSet, Vector2 mapPosition, int Columns, int Rows){
    tileset = &newTileSet;
    cell_size = tileset->tile_size;
    position = mapPosition;
    width = Columns;
    height = Rows;
    tilemap = new int[width * height];
    std::fill(tilemap, tilemap+width*height, -1);
    update = true;
    texture = LoadRenderTexture((int)((float)width*cell_size.x), (int)((float)height*cell_size.y));
}

TileMap::~TileMap(){
    delete [] tilemap;
    delete tileset;
    UnloadRenderTexture(texture);
}

void TileMap::ResizeMap(int left, int top, int right, int bottom){ //resize by cell size
    position.x += (float)left*cell_size.x;  //reposition horizontally
    position.y += (float)top*cell_size.y;   //reposition vertically
    int w = width -left +right;             //new width
    int h = height -top +bottom;            //new height
    //std::cout << "width: " << width << ' ' << height << ' ' << w << ' ' << h << std::endl;
    /* *********************/
    int* tmp = tilemap;                     //preparing for deleting old pointer
    tilemap = new int[w * h];           //new TileMap grid populated with -1(empty) indexes
    std::fill(tilemap, tilemap+w*h, -1);

    //move old map to new map grid
    int tile_max = tileset->tile_count;
    for (int y = 0; y < height; y++){
        if (y-top >= 0 && y-top < h){

            for (int x = 0; x< width; x++){
                    std::cout << x << ' ' << y << ' ' << tmp[width*y + x ] << std::endl;
                if (x-left >= 0 &&  x-left < w){

                    tilemap[w*(y-top) + x-left] = tmp[width*y + x ];
                }
            }
        }
    }
    delete [] tmp;
    tmp = NULL;                         //remove old array
    /* *****************************/

    width = w;                              //Save new width
    height = h;                             //save new height
    UnloadRenderTexture(texture);
    texture = LoadRenderTexture((int)((float)width*cell_size.x), (int)((float)height*cell_size.y)); //create new RenderTexture
    update = true;                          //set flag for render texture update
}

int TileMap::GetTileId(Vector2 CellPosition){
    bool lessX = CellPosition.x < 0.0f;
    bool lessY = CellPosition.y < 0.0f;
    bool biggerX = CellPosition.x > float(width);
    bool biggerY = CellPosition.y > float(height);
    if (lessX || lessY || biggerX || biggerY){
        return -1;
    }
    return tilemap[width*(int)CellPosition.y + (int)CellPosition.x];
}

int TileMap::GetTileIdWorld(Vector2 WorldPosition){
    bool lessX = WorldPosition.x < position.x;
    bool lessY = WorldPosition.y < position.y;
    bool biggerX = WorldPosition.x > position.x + cell_size.x * float(width);
    bool biggerY = WorldPosition.y > position.y + cell_size.y * float(height);
    if (lessX || lessY || biggerX || biggerY){
        return -1;
    }
    int x = int(WorldPosition.x - position.x)/width;
    int y = int(WorldPosition.y - position.y)/height;
    return tilemap[width*y + x];
}

void TileMap::SetPosition(Vector2 _position){
    position = _position;
}

Vector2 TileMap::World2Tile(Vector2 _position){
    float _x = floor((_position.x - position.x)/cell_size.x) * cell_size.x;
    float _y = floor((_position.y - position.y)/cell_size.y) * cell_size.y;
    return Vector2{_x, _y};
}

Vector2 TileMap::Tile2World(Vector2 _position){
    float _x = _position.x * cell_size.x + position.x;
    float _y = _position.y * cell_size.y + position.y;
    return Vector2{_x, _y};
}

void TileMap::Draw(){
    Update();

    DrawTextureRec(texture.texture, (Rectangle){ 0.0f, 0.0f,
                   (float)texture.texture.width, (float)-texture.texture.height },
                   position, WHITE);
}

void TileMap::Update(){
    if (!update){
        return;
    }

    BeginTextureMode(texture);  //bypassed if without RT
    DrawMapTiles();
    EndTextureMode();           //bypassed if without RT

    update = false;             //bypassed if without RT
}

void TileMap::DrawMapTiles(){
    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            tileset->draw_tile(tilemap[width*y + x], {x*cell_size.x, y*cell_size.y});
        }
    }
}

void TileMap::DrawMapPart(Vector2 _pos, Vector2  _size){    //TO-DO
    return;
    /*
    Vector2 w2t = World2Tile(_size);
    Vector2 tileSize = {w2t.x * cell_size.x, w2t.y * cell_size.y};
    Vector2 drawPosition = _pos - ceil(position/cell_size) * cell_size; //Rendering position against RenderTexture

    //Need first drawable tile
    int firstColumn = floor(drawPosition.x/cell_size.x)*cell_size.x;
    int firstRow    = floor(drawPosition.y/cell_size.y)*cell_size.y;

    DrawTextureV(RenderTexture.texture, -(position % cell_size)); //Draw RenderTexture
    */
}

void TileMap::Populate(){   //For Testing
    //test fill map
    int tile_max = tileset->tile_count;
    for(int i=0; i<width*height; i++){
        tilemap[i] = i % tile_max;
    }
}
