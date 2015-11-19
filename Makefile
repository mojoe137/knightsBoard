CC=g++
CFLAGS=-std=c++11 -Iinclude/ -Ofast
CFLAGSDEBUG=-std=c++11 -Iinclude/ -g
SRC=src/
BIN=bin/

all: knightsBoard
debug: knightsBoardDebug

knightsBoard: $(SRC)knightsBoard.c++ Map.o DistanceMap.o
	$(CC) $(CFLAGS) $(SRC)knightsBoard.c++ $(BIN)Map.o $(BIN)DistanceMap.o -o knightsBoard 

Map.o: $(SRC)Map.c++
	$(CC) $(CFLAGS) -c $(SRC)Map.c++ -o $(BIN)Map.o

DistanceMap.o: $(SRC)DistanceMap.c++
	$(CC) $(CFLAGS) -c $(SRC)DistanceMap.c++ -o $(BIN)DistanceMap.o 

knightsBoardDebug: $(SRC)knightsBoard.c++ MapDebug.o DistanceMapDebug.o
	$(CC) $(CFLAGSDEBUG) $(SRC)knightsBoard.c++ $(BIN)Map.o $(BIN)DistanceMap.o -o knightsBoard 

MapDebug.o: $(SRC)Map.c++
	$(CC) $(CFLAGSDEBUG) -c $(SRC)Map.c++ -o $(BIN)Map.o

DistanceMapDebug.o: $(SRC)DistanceMap.c++
	$(CC) $(CFLAGSDEBUG) -c $(SRC)DistanceMap.c++ -o $(BIN)DistanceMap.o 
