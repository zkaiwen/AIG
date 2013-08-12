/*@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@ 
 @
 @      vertex.hpp
 @      
 @      @AUTHOR:        Kevin Zeng
 @      Copyright 2012 – 2013 Virginia Polytechnic Institute and State University
 @
 @#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@*/


#ifndef VERTEX_GUARD
#define VERTEX_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

template<class T>
class Vertex
{
	private:
		T m_Type;
		int m_ID;    //Unique ID assigned
		int m_Index; //Position in the adjacency list;
		int m_Level;

		std::vector<Vertex<T>*> m_Input;	//Set of vertices connected into the current vertex
		std::vector<Vertex<T>*> m_Output;	//Set of vertices connected out of the current vertex
		std::vector<std::string> m_Parts;	//Set of subcircuits that are the same as the current

		unsigned int m_AliveAdj;
		
		/*****************
		* DATABASE
		*  0- unsolved
		*  1- alive
		*  2- dead
		* Graph
		*  0- unmarked
		*  1- marked
		*  2- tempmarked
		******************/
		char status;
					

	public:

Vertex(){
	m_Input.clear();
	m_Output.clear();
	status = 0;
	m_AliveAdj = 0;
	m_Index = 0;
	m_Level = -1;
}


Vertex(int vID, T type){
	m_Type = type;		
	m_ID = vID;
	m_Input.clear();
	m_Output.clear();
	status = 0;
	m_AliveAdj = 0;
	m_Index = 0;
	m_Level = -1;
}

Vertex(int vID){
	m_Type = "";		
	m_ID = vID;
	m_Input.clear();
	m_Output.clear();
	status = 0;
	m_AliveAdj = 0;
	m_Index = 0;
	m_Level = -1;
}

Vertex(const Vertex& rhs){
	printf("VERTEX CCONSTRUCTOR\n");
	m_Type = rhs.m_Type;
	m_ID = rhs.m_ID;
	status = rhs.status;
	m_AliveAdj = rhs.m_AliveAdj;
	
	for(unsigned int i = 0; i < rhs.m_Input.size(); i++)
		m_Input.push_back(rhs.m_Input[i]);
	for(unsigned int i = 0; i < rhs.m_Output.size(); i++)
		m_Output.push_back(rhs.m_Output[i]);
	for(unsigned int i = 0; i < rhs.m_Parts.size(); i++)
		m_Parts.push_back(rhs.m_Parts[i]);
}

Vertex<T>& operator=(const Vertex& rhs){
	printf("VERTEX = OPERATOR\n");
	m_Type = rhs.m_Type;
	m_ID = rhs.m_ID;
	status = rhs.status;
	m_AliveAdj = rhs.m_AliveAdj;
	
	for(unsigned int i = 0; i < rhs.m_Input.size(); i++)
		m_Input.push_back(rhs.m_Input[i]);
	for(unsigned int i = 0; i < rhs.m_Output.size(); i++)
		m_Output.push_back(rhs.m_Output[i]);
	for(unsigned int i = 0; i < rhs.m_Parts.size(); i++)
		m_Parts.push_back(rhs.m_Parts[i]);
	return *this;
}

int getNumParts(){
	return m_Parts.size();
}


void addInput(Vertex<T>* input){
	//printf("Adding... %d\n", input);
	m_Input.push_back(input);
}

void addOutput(Vertex<T>* output){
	//printf("Adding... %d\n", output);
	m_Output.push_back(output);
}

void removeOutput(int input){
	//printf("REMOVINGV... %d\n", input);
	m_Input.erase(m_Input.begin() + input);
}

bool removeInput(int inputVal){
	bool found = false;

	for(unsigned int i = 0; i < m_Input.size(); i++){
		if(m_Input[i]->getVertexID() == inputVal){
			m_Input.erase(m_Input.begin() + i);
			i--;
			found = true;
		}
	}
	return found;
}

void removeOutputValue(int outputVal){
	for(unsigned int i = 0; i < m_Output.size(); i++){
		if(m_Output[i]->getVertexID() == outputVal){
			m_Output.erase(m_Output.begin() + i);
			i--;
		}
	}
}

bool removeInputValue(int inputVal){
	bool found = false;

	for(unsigned int i = 0; i < m_Input.size(); i++){
		if(m_Input[i]->getVertexID() == inputVal){
			m_Input.erase(m_Input.begin() + i);
			i--;
			found = true;
		}
	}
	return found;
}

void getOutput(std::vector<Vertex<T>*> &v){
	v =  m_Output;
}

void getInput(std::vector<Vertex<T>*> &v){
	v = m_Input;
}

void addPart(std::string part){
	m_Parts.push_back(part);
}

void getPartList(std::vector<std::string>& part){
	part = m_Parts;
}

unsigned int getAliveAdj(){
	return m_AliveAdj;
}

void incrementAliveAdj(){
	m_AliveAdj++;
}

unsigned int getOVSize(){
	return m_Output.size();
}

unsigned int getIVSize(){
	return m_Input.size();
}

int getVertexID(){
	return m_ID;
}

int getVertexIndex(){
	return m_Index;
}

void setVertexID(int ID){
	m_ID = ID;
}

void setVertexIndex(int index){
	m_Index = index;
}

T getType(){
	return m_Type;
}

void setType(T type){
	m_Type = type;
}

void printNeighbors(){
	printf("V%d INPUTS:\t", m_ID);
	for(unsigned int i = 0; i < m_Input.size(); i++)
		printf("%d ", m_Input[i]->getVertexID());

	printf("\n");
	
	printf("V%d OUTPUTS:\t", m_ID);
	for(unsigned int i = 0; i < m_Output.size(); i++)
		printf("%d ", m_Output[i]->getVertexID());

	printf("\n");
}

void setStatus(char stat){
	status = stat;
}

char getStatus(){
	return status;
}

bool isNeighborSameType(){
	for(unsigned int i = 0; i < m_Output.size(); i++){
		std::vector<Vertex<T>*> in;
		m_Output[i]->getInput(in);

		//Find node that is a neigbor but not the same (Only single vertices
		if(in[0]->getVertexID() != m_ID && in[0]->getType()->getNumVertex() == 1){

			//If the type is the same, return false;
			if(in[0]->getType()->getSingleType() == m_Type->getSingleType()){
				return true; 
			}
		}
		else if(in[1]->getVertexID() != m_ID && in[1]->getType()->getNumVertex() == 1){

			//If the type is the same, return false;
			if(in[1]->getType()->getSingleType() == m_Type->getSingleType())
			{
				return true; 
			}
		}
	}
	return false;
}

void resizeOutput(int size){
	m_Output.reserve(size);

}

void resizeInput(int size){
	m_Input.reserve(size);

}

void setLevel(int level){
	m_Level = level;
}

int getLevel(){
	return m_Level;
}

};

#endif












