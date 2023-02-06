CC=g++
SRC=src/main.cpp
OBJ=build/obj/main.o
INCL=-I/usr/include/freetype2
LIBS=build/obj/glad.o -lGL -lglfw -lfreetype
BIN=build/bin/main

all: $(BIN) $(OBJ) $(SRC)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

$(OBJ): $(SRC)
	$(CC) -o $@ -c $^ $(LIBS)