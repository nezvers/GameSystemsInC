OUTPUT=main
CC=gcc
RAYLIB=../../raylib
CFLAGS=-Wall -std=c99 -Wno-missing-braces
LFLAGS=-I$(RAYLIB) -L$(RAYLIB) -lraylib -lopengl32 -lgdi32 -lwinmm

all: main

main: main.c
	$(CC) $(CFLAGS) -o $(OUTPUT).exe $^ $(RAYLIB)/raylib.rc.data $(LFLAGS)


clean:
	del *.o $(OUTPUT).exe