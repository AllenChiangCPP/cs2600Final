CC = gcc
MAIN = src/mqttTicTacToe.c
TEST = src/TicTacToeMod.c

all: program

program: src

	$(CC) -o bin/program -LC:\msys64\mingw64\share\licenses $(MAIN) -l paho-mqtt3c

program2: src

	$(CC) -o bin/testTTT $(TEST)

run: bin
	bin/program.exe

run2: bin
	bin/testTTT.exe