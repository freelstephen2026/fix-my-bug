CC=g++
SRC=src/main.cpp
OBJ=build/obj/main.o
LIBS=build/obj/glad.o -lGL -lglfw
BIN=build/bin/main

all: $(BIN) $(OBJ) $(SRC)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

$(OBJ): $(SRC)
	$(CC) -o $@ -c $^ $(LIBS)