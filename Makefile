# Makefile for pc game

build: pc

LIBS := libs
SOURCES := pc_game

pc: $(SOURCES)/pc.o $(LIBS)/minesweeper.o
	g++ -o $@ $^

$(SOURCES)/pc.o: $(SOURCES)/pc.cpp
	g++ -std=c++17 -c -I$(LIBS)/headers $^ -o $@

$(LIBS)/%.o: $(LIBS)/%.cpp
	g++ -std=c++17 -c -I$(LIBS)/headers $^ -o $@

clean:
	rm $(LIBS)/*.o $(SOURCES)/pc.o pc