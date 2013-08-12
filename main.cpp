/*@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@ 
 @
 @  MAIN.cpp
 @  
 @  @AUTHOR:Kevin Zeng
 @  Copyright 2012 – 2013 Virginia Polytechnic Institute and State University
 @
 @#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@*/


#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include "graph.hpp"
#include "vertex.hpp"
#include "aig.hpp"

int main( int argc, char *argv[] )
{
	/*if(argc != 3 && argc != 4)
	{
		printf("./fpgenius <input circuit file> <database file>\n");
		return 0;
	}
	*/
	std::string option = "";
	if(argc == 4)
		option = argv[3];

	struct stat st;
	std::ifstream infile;
	std::string file = "";


	if(stat(argv[1], &st)){
		perror(argv[1]);
		printf("Stat error...exiting\n");
		exit(1);
	}

	//Check to see if reference changed
	time_t now; 
	struct tm *current;
	now = time(0);
	current = localtime(&now);
	printf("[%d:%d:%d]\tChecking changes in reference...", current->tm_hour, current->tm_min, current->tm_sec);

	now = time(0);
	current = localtime(&now);
	printf("[%d:%d:%d]\tConverting graph to AIG...", current->tm_hour, current->tm_min, current->tm_sec);

	file = argv[1];
	AIG* aigraph = new AIG();
	Graph* graph = new Graph(file);
	graph->importGraph(file, 0);
	printf("Graph:\n");
	graph->printg();

	aigraph->convertGraph2AIG(graph);

	delete aigraph;
	delete graph;
	return 0;

}


