/*@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@
 @
 @      cutenumeration.cpp
 @      
 @      @AUTHOR:Kevin Zeng
 @      Copyright 2012 – 2013 Virginia Polytechnic Institute and State University
 @
 @#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@*/

#include "cutenumeration.hpp"

CutEnumeration::CutEnumeration(){
}



CutEnumeration::~CutEnumeration(){

}


void CutEnumeration::findKFeasibleCuts(AIG* graph, int k ){

	m_AIGraph = graph;

	//Start at the PO
	std::vector<int> output;
	graph->getOutputs(output);
	std::set<std::set<int> > dummy;

	for(unsigned int i = 0; i < output.size(); i++){
		cut(output[i], k, dummy)	
	}


}


void CutEnumeration::cut(int node, int k, std::set<std::set<int> >& cut){
	if(m_AIGraph[node] == NULL){
		std::set<int> singleSet;
		singleSet.insert(node);
		
	}

}


void CutEnumeration::merge(std::set<std::set<int> >& cut1, std::set<std::set<int> >& cut2, int k){


}




















