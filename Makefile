CC = gcc
MAIN = src/mqttTicTacToe.c

all: program

program: src

	$(CC) -o bin/program -LC:\msys64\mingw64\share\licenses mqttTicTacToe.c -l paho-mqtt3c

run: bin
	bin/program.exe