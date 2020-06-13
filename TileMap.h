
#ifndef TILEMAP_H_INCLUDED
#define TILEMAP_H_INCLUDED
#include "raylib.h"
#include "TileSet.h"

class TileMap{
    public:
        TileMap         (TileSet& newTileSet, Vector2 mapPosition, int Columns, int Rows);  //Construct
        ~TileMap        ();                                                                 //Deconstruct
        void            SetPosition(Vector2 _position);                                     //Change world position of the TileMap
        Vector2         GetPosition();                                                      //Get world position of the TileMap
        void            SetTile(Vector2 CellPosition, int index);                           //Set tile index in TileMap coordinates
        int             GetTile(Vector2 CellPosition);                                      //Get tile index in TileMap coordinates
        void            SetTileWorld(Vector2 WorldPosition, int index);                     //Set tile index in world coordinates
        int             GetTileWorld(Vector2 WorldPosition);                                //Get tile index in world coordinates
        Vector2         World2Tile(Vector2 _position);                                      //Convert world position into tile map cell position
        Vector2         Tile2World(Vector2 _position);                                      //Convert position of tile in the world
        void            Draw();                                                             //Draw TileMap in draw event
        void            Populate();                                                         //For test purposes fill all cells
    private:
        int*            tilemap;                                                            //1D Array used as 2D (y*width + x) to hold tile indexes
        TileSet*        tileset;                                                            //Tile set which tiles are used
        RenderTexture2D texture;                                                            //Texture used for oprimizing tile drawing
        Vector2         position;                                                           //World position of tile map
        int             width;                                                              //Horizontal cell count
        int             height;                                                             //Vertical cell count
        Vector2         cell_size;                                                          //Space taken by single tile

        bool            update;                                                             //Flag for render texture update
        void            Update();                                                           //Render texture update method
        void            DrawMapTiles();                                                     //Method that draws purely tiles with tilemap at (0, 0) position
        void            ResizeMap(int left, int top, int right, int bottom);                //resize map cells from border cells
        bool            TrimMap();                                                          //method to trim unused collumns and rows. Returns true if trimming happened
        void            DrawMapPart(Vector2 _pos, Vector2  _size);                          //TO-DO maybe
};





#endif // TILEMAP_H_INCLUDED
