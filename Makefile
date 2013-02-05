CXX=g++
CXXFLAGS=-g
LIBS=-lm
OBJECTS=main.o Plot.o Polygons.o Polygon.o
PROJECT=OvenSim

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LIBS) $(OBJECTS) -o $(PROJECT)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

Plot.o: Plot.cpp Plot.h
	$(CXX) $(CXXFLAGS) -c Plot.cpp -o Plot.o

Polygons.o: Polygons.cpp Polygons.h
	$(CXX) $(CXXFLAGS) -c Polygons.cpp -o Polygons.o

Polygon.o: Polygon.cpp Polygon.h
	$(CXX) $(CXXFLAGS) -c Polygon.cpp -o Polygon.o

clean:
	rm *.o $(PROJECT)
