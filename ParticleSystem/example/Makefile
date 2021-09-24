OUTPUT=main
CC=gcc
RAYLIB=../../raylib
INCLUDE=-I$(RAYLIB) -I../
CFLAGS=-Wall -std=c99 -Wno-missing-braces
LFLAGS=$(INCLUDE) -L$(RAYLIB) -lraylib -lopengl32 -lgdi32 -lwinmm

all: main

main: $(OUTPUT).c
	$(CC) $(CFLAGS) -o $(OUTPUT).exe $^ $(LFLAGS)


clean:
	del *.o $(OUTPUT).exe