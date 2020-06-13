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

    UnloadRenderTexture(texture);
    texture = LoadRenderTexture((int)((float)width*cell_size.x), (int)((float)height*cell_size.y)); //create new RenderTexture

    BeginTextureMode(texture);  //bypassed if without RT
    std::cout << "UPDATE" << std::endl;
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

int TileMap::GetTile(Vector2 CellPosition){
    bool lessX = CellPosition.x < 0.0f;
    bool lessY = CellPosition.y < 0.0f;
    bool biggerX = CellPosition.x > float(width);
    bool biggerY = CellPosition.y > float(height);
    if (lessX || lessY || biggerX || biggerY){
        return -1;
    }
    return tilemap[width*(int)CellPosition.y + (int)CellPosition.x];
}

void TileMap::SetTile(Vector2 CellPosition, int index){
    if (index < -1 || index > tileset->tile_count){ //Invalid index
        return;
    }
    int xp = (int)CellPosition.x;
    int yp = (int)CellPosition.y;
    bool xIn = xp >= 0 && xp < width;
    bool yIn = yp >= 0 && yp < height;
    if (xIn && yIn){
        if (tilemap[xp + yp * width] != index){
            tilemap[xp + yp * width] = index;
            if (index == -1){
                if(!TrimMap()){
                    update = true;
                }
            }
            else{
                update = true;
            }
            std::cout << "Set tile " << xp << ',' << yp << ' ' << tilemap[xp + yp * width] << std::endl;
        }
    }
    else if (index != -1){
        int left, top, right, bottom;
        left = xp < 0 ? xp : 0;
        top = yp < 0 ? yp : 0;
        right = xp >= width ? xp - (width-1) : 0;
        bottom = yp >= height ? yp - (height-1) : 0;
        ResizeMap(left, top, right, bottom);
        xp -= left;
        yp -= top;
        tilemap[xp + yp * width] = index;
    }
}

int TileMap::GetTileWorld(Vector2 WorldPosition){
    bool lessX = WorldPosition.x < position.x;
    bool lessY = WorldPosition.y < position.y;
    bool biggerX = WorldPosition.x > position.x + cell_size.x * float(width);
    bool biggerY = WorldPosition.y > position.y + cell_size.y * float(height);
    if (lessX || lessY || biggerX || biggerY){
        return -1;
    }
    int x = floor((WorldPosition.x - position.x)/cell_size.x);
    int y = floor((WorldPosition.y - position.y)/cell_size.y);
    return tilemap[width*y + x];
}

void TileMap::SetTileWorld(Vector2 WorldPosition, int index){
    SetTile(World2Tile(WorldPosition), index);
}

void TileMap::SetPosition(Vector2 _position){
    position = _position;
}

void TileMap::ResizeMap(int left, int top, int right, int bottom){ //resize by cell size
    position.x += (float)left*cell_size.x;  //reposition horizontally
    position.y += (float)top*cell_size.y;   //reposition vertically
    int w = width -left +right;             //new width
    int h = height -top +bottom;            //new height

    int* tmp = tilemap;                     //preparing for deleting old pointer
    tilemap = new int[w * h];           //new TileMap grid populated with -1(empty) indexes
    std::fill(tilemap, tilemap+w*h, -1);

    //move old map to new map grid
    for (int y = 0; y < height; y++){
        if (y-top >= 0 && y-top < h){

            for (int x = 0; x< width; x++){
                if (x-left >= 0 &&  x-left < w){
                    tilemap[w*(y-top) + x-left] = tmp[width*y + x ];
                }
            }
        }
    }
    delete [] tmp;
    tmp = NULL;                         //remove old array

    width = w;                              //Save new width
    height = h;                             //save new height
    update = true;                          //set flag for render texture update
}

bool TileMap::TrimMap(){
    int left = width-1;
    int top = height-1;
    int right = 0;
    int bottom = 0;
    for (int y=0; y<height; y++){
        for (int x=0; x<width; x++){
            if (tilemap[width*y + x] > -1){
                if (x < left) left = x;
                if (y < top) top = y;
                if (x > right) right = x;
                if (y > bottom) bottom = y;
            }
        }
    }
    left -= 0;
    top -= 0;
    right -= (width-1);
    bottom -= (height-1);
    if (left!=0 || top!=0 || right!=0 || bottom!=0){
        ResizeMap(left,top,right,bottom);
        std::cout << "Trimmed "<< left << ' ' << top << ' ' << right << ' ' << bottom << std::endl;
        return true;
    }
    else{
        return false;
    }
}

Vector2 TileMap::World2Tile(Vector2 _position){
    float xp = floor((_position.x-position.x)/cell_size.x);
    float yp = floor((_position.y-position.y)/cell_size.y);
    return (Vector2){xp, yp};
}

Vector2 TileMap::Tile2World(Vector2 _position){
    float _x = _position.x * cell_size.x + position.x;
    float _y = _position.y * cell_size.y + position.y;
    return (Vector2){_x, _y};
}

void TileMap::DrawMapPart(Vector2 _pos, Vector2  _size){    //TO_DO useful for frustrum culling
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
