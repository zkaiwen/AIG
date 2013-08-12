/*@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@
 @
 @	graph.hpp
 @	
 @	@AUTHOR:	Kevin Zeng
 @      Copyright 2012 – 2013 Virginia Polytechnic Institute and State University
 @
 @#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@*/

#ifndef GRAPH_GUARD
#define GRAPH_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include <queue>
#include <string>
#include "vertex.hpp"

class Graph
{
	private:
		//Main datastructure (int: vertex label)
		std::map<int, Vertex<std::string>*> m_GraphV;
		std::vector<int> m_Inputs;
		std::map<std::string, std::vector<Vertex<std::string>*> > m_numType;
		std::vector<Graph*> m_ConnectedGraphs;

		std::string m_Name;
		int m_Last;
		int m_NumInputs;
		int m_NumEdges;
		int m_MaxLevel;
		bool m_Disconnected;	

		std::vector<std::vector<int>* > F;
		std::vector<std::vector<int>* > E;
		std::map<int,int> vmap;

		void removeInputs();
		
	public:	
		Graph(std::string);
		~Graph();

		void getIterators(typename std::map<int, Vertex<std::string>*>::iterator&, typename std::map<int,Vertex<std::string>*>::iterator&);
		typename std::map<int, Vertex<std::string>*>::iterator getBeginIterator();
		typename std::map<int, Vertex<std::string>*>::iterator getEndIterator();
		void getNumType(std::map<std::string, std::vector<Vertex<std::string>*> >&);
		Vertex<std::string>* getVertex(int);
		unsigned int getNumVertex();
		unsigned int getNumEdges();
		unsigned int getNumInputs();
		std::string getName();
		std::string getSingleItem();
		std::string getSingleType();
		void getInputs(std::vector<int>&);
		void getConnectedGraphs(std::vector<Graph*>&);
		int getNumGate(std::string);
		int getVertexIDFromIndex(int);
		int getLast();
		int getMaxLevel();
		
		void setNumType(std::map<std::string, std::vector<Vertex<std::string>*> >&);
		void setName(std::string);
		void setNumEdges(unsigned int);
		void setNumInputs(unsigned int);
		void setLast(int);
		void setLevels();

		void substitute(int, Graph*);


		void addVertex(Vertex<std::string>*);
		Vertex<std::string>* addVertex(int, std::string);
		Vertex<std::string>* addVertex(int);
		Vertex<std::string>* addVertex(int, Vertex<std::string>*);

		void removeVertex(Vertex<std::string>*);
		void removeVertex(int);

	



		void remapVertex(int, int);
		void remapEdgeSet(int, int);
		void remapVMap(int, int);
		void disconnectVertices();
	




		bool importGraph(std::string, int);
		bool difference(std::vector<int>&, std::set<int>&);
		void addIsomorphism(std::vector<int>*);
		void addIsomorphism(std::vector<std::vector<int>*>&);
		void addEdgeSet(std::vector<int>*);
		void addEdgeSetCopy(std::vector<int>*);
		void addIsomorphismSwap(std::vector<int>&);
		void addEdgeSetSwap(std::vector<int>*);
		void getEdgeSet(std::vector<std::vector<int>* >&);
		void getIsomorphismSet(std::vector<std::vector<int>* >&);
		void getVMap(std::map<int,int>&);
		void setVMap(std::vector<int>*, std::vector<int>*);
		void setVMapSwap(std::map<int,int>);
		bool isAdjacent_directed(int, int);
		bool isAdjacent(int, int);
		bool isConnected(bool);

		void DFS(Vertex<std::string>*, std::map<int, bool>&, unsigned int );
		void DFS(Vertex<std::string>*, std::map<int, bool>&, std::set<int>&);
		void DFS(Vertex<std::string>*, std::map<int, bool>&);
		void BFSLevel(std::queue<Vertex<std::string>*, std::list<Vertex<std::string>*> >, std::map<int,bool>&, int, int);

		void resetStatus();
		void clearF();
		void resizeE(int);
		void resizeF(int);
		
		void print();
		void printg();
		void printI();
		void printV(std::string, std::vector<int>&);
		void printV(std::string, std::vector<int>*);
		void print2DV(std::string, std::vector<std::vector<int> >&);
		void print2DV(std::string, std::vector<std::vector<int>* >&);
};

#endif
