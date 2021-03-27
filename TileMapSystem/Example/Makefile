CC=gcc
RAYLIB=../../raylib
CFLAGS=-Wall -std=c99 -Wno-missing-braces
LFLAGS=-I$(RAYLIB) -L$(RAYLIB) -lraylib -lopengl32 -lgdi32 -lwinmm

all: Test_AutotTile Test_TileMap Test_TileMap_spread_parallax Test_TileSet_bitmask_editor

Test_AutotTile: Test_AutotTile.c
	$(CC) $(CFLAGS) -o $@ $^ $(RAYLIB)/raylib.rc.data $(LFLAGS)

Test_TileMap: Test_TileMap.c
	$(CC) $(CFLAGS) -o $@ $^ $(RAYLIB)/raylib.rc.data $(LFLAGS)

Test_TileMap_spread_parallax: Test_TileMap_spread_parallax.c
	$(CC) $(CFLAGS) -o $@ $^ $(RAYLIB)/raylib.rc.data $(LFLAGS)

Test_TileSet_bitmask_editor: Test_TileSet_bitmask_editor.c
	$(CC) $(CFLAGS) -o $@ $^ $(RAYLIB)/raylib.rc.data $(LFLAGS)




clean:
	del *.o $(OUTPUT).exe