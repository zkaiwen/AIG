CFLAGS = \
		-Wall \
		-g \
		-O3
		

OBJ= \
		graph.o \
		aig.o \
		main.o

PROF=

all: main

main: $(OBJ) main.o  
	g++ $(CFLAGS) -o xfpgenius $(OBJ) $(PROF) -lboost_graph

graph.o: graph.cpp graph.hpp vertex.hpp
	g++ $(CFLAGS) -c graph.cpp $(PROF)

main.o: main.cpp graph.o aig.o
	g++ $(CFLAGS) -c main.cpp $(PROF)

aig.o: aig.cpp aig.hpp
	g++ $(CFLAGS) -c aig.cpp $(PROF)

clean:
	rm -f *.o fpgenius *.out 

