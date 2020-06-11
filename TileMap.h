
#ifndef TILEMAP_H_INCLUDED
#define TILEMAP_H_INCLUDED
#include "raylib.h"
#include "TileSet.h"

class TileMap{
    public:
        TileMap(TileSet& newTileSet, Vector2 mapPosition, int Columns, int Rows);
        ~TileMap();
        void SetMapSize(int left, int top, int right, int bottom);
        void SetPosition(Vector2 _position);
        int GetTileId(Vector2 CellPosition);
        int GetTileIdWorld(Vector2 WorldPosition);
        Vector2 World2Tile(Vector2 _position);
        Vector2 Tile2World(Vector2 _position);
        void Draw();
        void Populate();
    private:
        int* tilemap;
        TileSet* tileset;
        RenderTexture2D texture;
        Vector2 position;
        int width;
        int height;
        Vector2 cell_size;
        bool update;                //Flag for render texture update
        void Update();              //Render texture update
        void DrawMapTiles();
        void DrawMapPart(Vector2 _pos, Vector2  _size);
};





#endif // TILEMAP_H_INCLUDED
