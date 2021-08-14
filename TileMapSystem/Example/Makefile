CC=gcc
RAYLIB=../../raylib°
INCLUDE=-I$(RAYLIB) -I../
CFLAGS=-Wall -std=c99 -Wno-missing-braces
LFLAGS=$(INCLUDE) -L$(RAYLIB) -lraylib -lopengl32 -lgdi32 -lwinmm

all: Test_AutotTile Test_TileMap Test_TileMap_spread_parallax Test_TileSet_bitmask_editor

Test_AutotTile: Test_AutotTile.c
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

Test_TileMap: Test_TileMap.c
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

Test_TileMap_spread_parallax: Test_TileMap_spread_parallax.c
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

Test_TileSet_bitmask_editor: Test_TileSet_bitmask_editor.c
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)




clean:
	del *.o $(OUTPUT).exe