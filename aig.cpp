/*@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@
 @
 @      aig.cpp
 @      
 @      @AUTHOR:Kevin Zeng
 @      Copyright 2012 – 2013 Virginia Polytechnic Institute and State University
 @
 @#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@*/


#include "aig.hpp"

AIG::AIG(){
}

AIG::~AIG(){
	for(unsigned int i = 0; i < m_AIG.size(); i++){
		delete [] m_AIG[i];	
	}
	std::map<int*, int, cmp_intp>::iterator it;
	for(it = m_HashTable.begin(); it!= m_HashTable.end(); it++)
		delete [] it->first;
}

/*
void AIG::importAIG(const char* fileName, std::vector<std::vector<int>*>& aig){
			
	std::ifstream inFile;
	inFile.open(fileName);
	if (!inFile.is_open())	{
		fprintf(stderr, "[ERROR] -- Cannot open the file %s for import...exiting\n", fileName);
		fprintf(stderr, "\n***************************************************\n\n");
		exit(-1);
	}

	printf("Reading in AIG information for %s\n", fileName);
	
	int numNodes;

	inFile >> numNodes;
	printf("Number of Nodes:\t%d\n", numNodes);
	std::vector<int>* dummy = NULL;
	aig.push_back(dummy);
	aig.push_back(dummy);

	for(int i = 0; i < numNodes; i++){
		int vID;
		int fanout;
	
		//Does not store anything in the zero index
		inFile >> vID;	
		vID+=2;
		printf("VID:\t%d\n", vID);

		inFile >> fanout;
		std::vector<int>* outputs = new std::vector<int>();
		outputs->reserve(fanout);
		printf("Number of Outputs:\t%d\n", fanout);

		for(int j = 0; j < fanout; j++){
			int output;
			inFile >> output;

			//Handles negative edges
			if(output < 0)
				output-=2;
			else
				output+=2;

			printf("%d ", output);
			outputs->push_back(output);
		}
		printf("\n");

		aig.push_back(outputs);
	}

	printf("Import Complete***********\n\n");
}
*/


void AIG::convertGraph2AIG(Graph* ckt){
	//Import primitives
	const char* xorAIGFile_c = "circuits/g/primitive/xor.g";
	const char* orAIGFile_c = "circuits/g/primitive/or.g";
	const char* norAIGFile_c = "circuits/g/primitive/nor.g";
	const char* nandAIGFile_c = "circuits/g/primitive/nand.g";



	int size = ckt->getNumVertex();
	std::map<int, Vertex<std::string>*>::iterator it;
	std::vector<Vertex<std::string>*> gNodeOutput;
	
	//printf("BEFORE SUBSTITUTION\n");
	//ckt->printg();
	std::vector<int> toBeDeleted;

	for(it = ckt->getBeginIterator(); it != ckt->getEndIterator(); it++){
		const char* circuitType;
		//printf("VERTEX: %d\n", it->first);
		//Place the necessary nodes into the table
		if(it->second->getType() == "XORX")
			circuitType = xorAIGFile_c;
		else if(it->second->getType() == "ORX")
			circuitType = orAIGFile_c;
		else if(it->second->getType() == "NORX")
			circuitType = norAIGFile_c;
		else if(it->second->getType() == "NANDX")
			circuitType = nandAIGFile_c;
		else
			continue;
		
			//printf("FOUND COMPLETE PRIMITIVE:\t%s\n", circuitType);

			Graph* graph = new Graph(circuitType);
			graph->importGraph(circuitType, size);
			size+= graph->getNumVertex();
			
			ckt->substitute(it->first, graph);
			toBeDeleted.push_back(it->first);
	}
	
	//printf("Deleting vertices\n");
	for(unsigned int i = 0; i < toBeDeleted.size(); i++)
		ckt->removeVertex(toBeDeleted[i]);

	//printf("AFTER SUBSTITUTION\n");
	//ckt->printg();
	//printf("***********************************************\n\n");




	
	/******************************************************
	*
	*
	* PERFORM AIG CALCULATION
	*
	*
	*******************************************************/

	//Set Levels
	ckt->setLevels();
	//ckt->print();
	
	//CONSTANT 0 AND 1
	int* dummy = NULL;
	std::vector<int> gateMap(size+10, 0);
	m_AIG.reserve(ckt->getNumVertex()+5);
	m_AIG.push_back(dummy);
	m_AIG.push_back(dummy);
	

	//Order the vertices by level
	std::map<int, std::vector<Vertex<std::string>*> > vLevel;
	for(it = ckt->getBeginIterator(); it != ckt->getEndIterator(); it++)
		vLevel[it->second->getLevel()].push_back(it->second);

	//Create nodes for input
	for(unsigned int i = 0; i < vLevel[0].size(); i++){
		gateMap[vLevel[0][i]->getVertexID()] = create_input();
	}
	



	//Create the rest of the AIG Graph
	for(unsigned int i = 1; i < vLevel.size(); i++){
		//printf("\nLEVEL %d, SIZE: %d\n", i, vLevel[i].size());
		for(unsigned int j = 0; j < vLevel[i].size(); j++){
			//printf("VERTEX ID: %d\n", vLevel[i][j]->getVertexID());
			if(vLevel[i][j]->getType() == "ANDX"){
				//printf("GATE: AND\n");
				std::vector<Vertex<std::string>*> in;
				vLevel[i][j]->getInput(in);
				//printf("VIN: %d %d\n", in[0]->getVertexID(), in[1]->getVertexID());


				int output = create_and2(gateMap[in[0]->getVertexID()], gateMap[in[1]->getVertexID()]);

				gateMap[vLevel[i][j]->getVertexID()] = output;
				

			}
			else if(vLevel[i][j]->getType() == "INV"){
				//printf("GATE: INVERTER\n");
				std::vector<Vertex<std::string>*> in;
				vLevel[i][j]->getInput(in);
				//printf("VIN: %d\n", in[0]->getVertexID());
				gateMap[vLevel[i][j]->getVertexID()] = gateMap[in[0]->getVertexID()] * -1;
			}
			else
			{
				printf("Unknown Gate type:\t%s\n", vLevel[i][j]->getType().c_str());
				exit(1);
			}

			//printf("\n*#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@\n\n\n\n");
			
		}
	}
	
	
	//Store primary output nodes
	int maxLevel = vLevel.size() - 1;
	printf("MAXLEVEL: %d\n", maxLevel);
	for(unsigned int i = 0 ; i < vLevel[vLevel.size()-1].size(); i++){
		m_Outputs.push_back(gateMap[vLevel[maxLevel][i]->getVertexID()]);
	}

	for(unsigned int i = 0; i < m_Outputs.size(); i++)
		printf("%d ", m_Outputs[i]);
	


	/*for(unsigned int i = 0; i < gateMap.size(); i++){
		printf("GRAPHV: %d \tAIG: %d\n", i, gateMap[i]);
	}*/

	print();
}

int AIG::create_and2(int e1, int e2){
	//printf("e1: %d\te2:%d\n", e1, e2);
	if(e1 == 0)   return 0;
	if(e2 == 0)   return 0;
	if(e1 == 1)   return e2;
	if(e2 == 1)   return e1;
	if(e2 == e1)  return e1;
	if(e1 == e1*-1) return 0;
	
	if(e1 > e2)   std::swap(e1,e2);

	int* mapkey = new int[2];
	mapkey[0] = e1;
	mapkey[1] = e2;
	
	int newEdge;	
	if(m_HashTable.find(mapkey) == m_HashTable.end())
		newEdge = new_and_vertex(e1, e2);
	else
		newEdge = m_HashTable[mapkey];
		
	delete [] mapkey;
	
	//printf("AND created with node: %d. SIZE: %d\n\n", newEdge, m_AIG.size());

	return newEdge;
}


int AIG::create_input(){
	//Create an empty vertex
	int* const inputPair = NULL;
	m_AIG.push_back(inputPair);

	//Return location of created input
	//printf("Input created with node: %d. SIZE: %d\n\n", m_AIG.size()-1, m_AIG.size());
	return m_AIG.size() - 1;
}


int AIG::new_and_vertex(int e1, int e2){
	//Index of new vertex
	int newEdge = m_AIG.size();

	//Inputs to the current vertex
	int* const inputPair = new int[2];
	//printf("KEY: %d %d\tVALUE:%d\n", e1, e2, newEdge);
	inputPair[0] = e1;
	inputPair[1] = e2;
	m_AIG.push_back(inputPair);

	//Put vertex into hashtable
	m_HashTable[inputPair] = newEdge;
	return newEdge;
}





void AIG::printHash(){
	std::map<int*, int, cmp_intp>::iterator it;
	printf("Printing Hash Table\tSIZE: %d\n", (int) m_HashTable.size());
	for(it = m_HashTable.begin(); it!= m_HashTable.end(); it++)
		if(it->first != NULL)
			printf("KEY: %d %d\tVAL: %d\n", it->first[0], it->first[1], it->second);
		else
			printf("KEY IS NULL\n");
}



void AIG::print(){
	printf("\n\nPrinting AIG...********************************\n");
	for(unsigned int i = 2; i < m_AIG.size(); i++){
		if(m_AIG[i] != NULL)
			printf("V%d\tIN: %d %d\n", i, m_AIG[i][0], m_AIG[i][1]);
		else
			printf("V%d\tINPUT\n", i);
	}
}







/**************************************************************
*
*     Getter Methods
*
*
***************************************************************/

void AIG::getOutputs(std::vector<int>& output){
	output = m_Outputs;
}
