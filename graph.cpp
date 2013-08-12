/*@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@
 @
 @      graph.cpp
 @      
 @      @AUTHOR:Kevin Zeng
 @      Copyright 2012 – 2013 Virginia Polytechnic Institute and State University
 @
 @#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@*/

#include "graph.hpp"
Graph::Graph(std::string name){
	m_Name = name;
	m_Last = 0;
	m_NumInputs = 0;
	m_NumEdges = 0;
	m_Disconnected = false;
}

/*
Graph(const Graph original){
	if(original == NULL)
		return NULL;
	
	
	typename std::map<int, Vertex<std::string>*>::iterator it;

	for(it = original.m_GraphV.begin(); it != original.m_GraphV.end(); it++){
		Vertex<std::string>* v = it->second;
		m_GraphV[it->first] = v;
	}

	for(unsigned int i = 0; i < original.m_Inputs.size(); i++)
		m_Inputs.push_back(original.m_Inputs[i]);

}
*/

Graph::~Graph(){
		typename std::map<int, Vertex<std::string>*>::iterator it; 
		for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
			if(it->second != NULL)
				delete it->second;
		}
	//printf("Memory free\n");

	for(unsigned int i = 0; i < F.size(); i++)
		delete F[i];
	for(unsigned int i = 0; i < E.size(); i++)
		delete E[i];
	for(unsigned int i = 0; i < m_ConnectedGraphs.size(); i++)
		delete m_ConnectedGraphs[i];
	
}

void Graph::disconnectVertices(){
	m_Disconnected = true;
}



int Graph::getVertexIDFromIndex(int index){
	typename std::map<int, Vertex<std::string>*>::iterator it; 
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		if(it->second->getVertexIndex() == index)
			return it->second->getVertexID();
	}
	return -1;
}




Vertex<std::string>* Graph::getVertex(int vertex){
	return m_GraphV[vertex];
}

unsigned int Graph::getNumVertex(){
	return m_GraphV.size();
}

unsigned int Graph::getNumInputs(){
	//printf("numINPUTS: %d\n", m_NumInputs);
	return m_NumInputs;
}

void Graph::getInputs(std::vector<int>& input){
	input = m_Inputs;
}

void Graph::setNumInputs(unsigned int inputs){
	m_NumInputs = inputs;
}



void Graph::setName(std::string name){
	m_Name = name ;
}

std::string Graph::getName(){
	return m_Name;
}


int Graph::getLast(){
	return m_Last;
}

void Graph::setLast(int last){
	m_Last = last;
}
		
		
void Graph::getConnectedGraphs(std::vector<Graph*>& cgraph){
	cgraph = m_ConnectedGraphs;
}


Vertex<std::string>* Graph::addVertex(int vID, std::string type){
	Vertex<std::string> *v = new Vertex<std::string>(vID, type);
	v->setVertexIndex(m_GraphV.size());
	m_GraphV[vID] = v;

	//printf("VID: %d MLAST %d\n", vID, m_Last);
	return v;
}

Vertex<std::string>* Graph::addVertex(int vID){
	Vertex<std::string> *v = new Vertex<std::string>(vID);
	v->setVertexIndex(m_GraphV.size());
	m_GraphV[vID] = v;

	//printf("VID: %d MLAST %d\n", vID, m_Last);
	return v;
}

Vertex<std::string>* Graph::addVertex(int vID, Vertex<std::string>* v){
	v->setVertexIndex(m_GraphV.size());
	m_GraphV[vID] = v;
	return v;
}

void Graph::removeVertex(Vertex<std::string>* vertex){
	int id = vertex->getVertexID();
	delete m_GraphV[id];
	m_GraphV.erase(id);
	
}

void Graph::removeVertex(int vertex){
	delete m_GraphV[vertex];
	m_GraphV.erase(vertex);
}


/*
void merge(std::vector<int>& nodes, std::string type){
	Vertex<std::string>* newNode =  new Vertex<std::string>(m_Last, (char*) type.c_str());
	std::vector<int> nodeIn;
	std::vector<int> nodeOut;

	//Set up new vertex
	m_GraphV[m_Last] = newNode;
	m_GraphV[m_Last]->getInput(nodeIn);		
	m_GraphV[m_Last]->getOutput(nodeOut);		

	//Collapse all the nodes in the list to one node. 
	for(unsigned int i = 0; i < nodes.size(); i++){
		std::vector<int> in;
		std::vector<int> out;

		//Get in and out of node to collapse
		m_GraphV[nodes[i]]->getInput(in);		
		m_GraphV[nodes[i]]->getOutput(out);		


		//Add input to new node if input is not one of the collapsed
		for(unsigned ii = 0; ii < in.size(); ii++){
			bool found = false;
			for(unsigned int j = 0; j < nodes.size(); j++)
				if(nodes[j] == in[ii]){
						found = true;
						break; 
				}

			if(found)
				continue;

			newNode->addInput(in[ii]);	
			in[ii] = m_Last;
		}
		
		//Add output to new node if output is not one of the connected
		for(unsigned ii = 0; ii < out.size(); ii++){
			bool found = false;
			for(unsigned int j = 0; j < nodes.size(); j++)
				if(nodes[j] == out[ii]){
						found = true;
						break; 
				}

			if(found)
				continue;

			newNode->addOutput(out[ii]);	
			out[ii] = m_Last;
		}

		//Erase the vertex from the list
		m_GraphV.erase(nodes[i]);
	}		

	typename std::map<int, Vertex<std::string>*>::iterator it;
	for(it = m_GraphV.begin(); it!= m_GraphV.end(); it++){
		std::vector<int> in;
		std::vector<int> out;
		it->second->getInput(in);		
		it->second->getOutput(out);		

		for(unsigned int i = 0; i < in.size(); i++)
			for(unsigned int j = 0; j < nodes.size(); j++)
				if(in[i] == nodes[j]){
					it->second->removeInput(in[i]);
					it->second->addInput(m_Last);

					
					break;
				}
		
		for(unsigned int i = 0; i < out.size(); i++)
			for(unsigned int j = 0; j < nodes.size(); j++)
				if(out[i] == nodes[j]){
					it->second->removeOutput(out[i]);
					it->second->addOutput(m_Last);
					
					break;
				}
	}
//	for(it = m_Inputs.begin(); it!= m_Inputs.end(); it++){
//		std::vector<int> out;
//		it->second->getOutput(out);		
//
//		for(unsigned int i = 0; i < out.size(); i++)
//			for(unsigned int j = 0; j < nodes.size(); j++)
//				if(out[i] == nodes[j]){
//					it->second->removeOutput(out[i]);
//					it->second->addOutput(m_Last);
//					
//					break;
//				}
//	}
	m_Last++;
}

}*/

		typename std::map<int, Vertex<std::string>*>::iterator Graph::getBeginIterator(){
	return m_GraphV.begin();
}

		typename std::map<int, Vertex<std::string>*>::iterator Graph::getEndIterator(){
	return m_GraphV.end();
}


void Graph::getIterators(typename std::map<int, Vertex<std::string>*>::iterator& begin, typename std::map<int,Vertex<std::string>*>::iterator& end ){
	begin = m_GraphV.begin();
	end = m_GraphV.end();
}

bool Graph::importGraph(std::string fileName, int offset){

	std::ifstream inFile;
	inFile.open(fileName.c_str());
	if (!inFile.is_open())	{
		fprintf(stderr, "[ERROR] -- Cannot open the file %s for import...exiting\n", fileName.c_str());
		fprintf(stderr, "\n***************************************************\n\n");
		exit(-1);
	}

	//printf("Reading in circuit information for %s\n", fileName.c_str());
	
	int numNodes;
	int numInputNodes;

	inFile >> numNodes >> numInputNodes;
	m_NumInputs = numInputNodes;

	//printf("Reading in input information...\n");
	for(int i = 0; i < numInputNodes; i++){
		int input; 
		inFile >> input;
		m_Inputs.push_back(input + offset);
	}

	
	//printf("Reading in gate information...\n");
	for(int i = 0; i < numNodes; i++){
		int vID;
		std::string type;
		int fanin;
		int fanout;
	
		inFile >> vID;	
		vID += offset;
		//printf("VID: %d\n", vID);

		inFile >> type;
		//printf("ID: %d\tTYPE: %s\n", vID, type.c_str());
		Vertex<std::string>* vertex;
			
/*		if((std::string) type == "IN"){
			numInputs++;
			vertex = addInput(vID + last, type);
		}
		else*/
		if(m_GraphV.find(vID) == m_GraphV.end())
			vertex = addVertex(vID, type);
		else{
			vertex = m_GraphV[vID];
			vertex->setType(type);
		}


		//Keep track of how many of each type there are
		std::string typestr = type;
		if(typestr != "IN"){
			m_numType[typestr].push_back(vertex);
		}
		
		inFile >> fanin;
		vertex->resizeInput(fanin);
		for(int j = 0; j < fanin; j++){
			int input;
			inFile >> input;
			input += offset;
			
			Vertex<std::string>* in;
			if(m_GraphV.find(input) == m_GraphV.end())
				in = addVertex(input);
			else
				in = m_GraphV[input];	
			vertex->addInput(in);
		}
		
		inFile >> fanout;
		vertex->resizeOutput(fanout);
		for(int j = 0; j < fanout; j++){
			int output;
			inFile >> output;
			output += offset;
			
			Vertex<std::string>* out;
			if(m_GraphV.find(output) == m_GraphV.end())
				out = addVertex(output);
			else
				out = m_GraphV[output];	
			vertex->addOutput(out);
			m_NumEdges++;
		}
	}

	printf("Import Complete\n");
	printf("NUMV: %d\n", (unsigned int) m_GraphV.size());
	printf("NUME: %d\n", m_NumEdges);
	return true;
}
	
	
void Graph::print(){
	typename std::map<int, Vertex<std::string>*>::iterator it;
	/*for(it = m_Inputs.begin(); it != m_Inputs.end(); it++){
		printf("V%d\tL:%d\tT: %s ", it->first, it->second->getLevel(), it->second->getType().c_str());
		
		printf("\tOUT: ");
		std::vector<int> out;
		it->second->getOutput(out);
		for(unsigned int i = 0; i < out.size(); i++)
			printf("%d ", out[i]);
		printf("\n");
	}*/

	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		printf("V%d\tL:%d\tT: %s\tIN: ", it->first, it->second->getLevel(),it->second->getType().c_str());
		std::vector<Vertex<std::string>*> in;
		it->second->getInput(in);
		for(unsigned int i = 0; i < in.size(); i++)
			printf("%d ", in[i]->getVertexID());
		
		printf("\tOUT: ");
		std::vector<Vertex<std::string>*> out;
		it->second->getOutput(out);
		for(unsigned int i = 0; i < out.size(); i++)
			printf("%d ", out[i]->getVertexID());
		printf("\n");
	}

	printf("\n\n");
}	

void Graph::printg(){
	typename std::map<int, Vertex<std::string>*>::iterator it;
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		printf("%d %s ", it->first, it->second->getType().c_str());
		std::vector<Vertex<std::string>*> in;
		it->second->getInput(in);
		printf(" %d ", (unsigned int) in.size());
		for(unsigned int i = 0; i < in.size(); i++)
			printf("%d ", in[i]->getVertexID());
		
		std::vector<Vertex<std::string>*> out;
		it->second->getOutput(out);
		printf(" %d ", (unsigned int)out.size());
		for(unsigned int i = 0; i < out.size(); i++)
			printf("%d ", out[i]->getVertexID());
		printf("\n");
	}

	printf("\n\n");
}	

void Graph::printI(){
	typename std::map<int, Vertex<std::string>*>::iterator it;
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		printf("%d %s ", it->second->getVertexIndex(), it->second->getType().c_str());
		std::vector<Vertex<std::string>*> in;
		it->second->getInput(in);
		printf(" %d ", (unsigned int) in.size());
		for(unsigned int i = 0; i < in.size(); i++)
			printf("%d ", in[i]->getVertexIndex());
		
		std::vector<Vertex<std::string>*> out;
		it->second->getOutput(out);
		printf(" %d ", (unsigned int)out.size());
		for(unsigned int i = 0; i < out.size(); i++)
			printf("%d ", out[i]->getVertexIndex());
		printf("\n");
	}

	printf("\n\n");
}	


void Graph::setNumType(std::map<std::string, std::vector<Vertex<std::string>*> >& numtype){
	m_numType = numtype;
}

void Graph::getNumType(std::map<std::string, std::vector<Vertex<std::string>*> >& numtype){
	numtype = m_numType;
}

void Graph::addIsomorphism(std::vector<int>* iso){
	F.push_back(iso);
	//printf("SIZE OF ISO SET: %d\n", F.size());
}

void Graph::addIsomorphism(std::vector<std::vector<int>*>& iso){
	//printf("aF cap: %d\n", F.capacity());
	//printf("aF newcap: %d\n", F.capacity());
	//printf("aF size: %d\n", iso.size());
	for(unsigned int i = 0; i < iso.size(); i++)
		F.push_back(iso[i]);
	//printf("aF finalcap: %d\n\n", F.capacity());
	//printf("SIZE OF ISO SET: %d\n", F.size());
}

void Graph::addEdgeSet(std::vector<int>*edges){
	E.push_back(edges);
	//printf("SIZE OF EDGE SET: %d\n", E.size());
}

void Graph::addEdgeSetCopy(std::vector<int>*edges){
	std::vector<int>* newCopy = new std::vector<int>();
	newCopy->push_back(edges->at(0));
	newCopy->push_back(edges->at(1));
	E.push_back(newCopy);
	//printf("SIZE OF EDGE SET: %d\n", E.size());
}

void Graph::addIsomorphismSwap(std::vector<int>& iso){
	std::vector<int> newIso;
	newIso.swap(iso);
	//F.push_back(newIso);
	//printf("aF size: %d\n", F.size());
	//printf("aF CAP: %d\n", F.capacity());
	//printf("SIZE OF ISO SET: %d\n", F.size());
}

void Graph::addEdgeSetSwap(std::vector<int>* edges){
	E.push_back(edges);
	//printf("SIZE OF EDGE SET: %d\n", E.size());
}

void Graph::getEdgeSet(std::vector<std::vector<int>* >& edges){
	edges = E;
}

void Graph::getIsomorphismSet(std::vector<std::vector<int>* >& iso){
	iso = F;
}

void Graph::getVMap(std::map<int, int>& mapping){
	mapping = vmap;
}

void Graph::setVMap(std::vector<int>* original, std::vector<int>* newMapping){
}


void Graph::setVMapSwap(std::map<int, int> newMapping){
	vmap = newMapping;

}

bool Graph::isAdjacent_directed(int v1, int v2){
	//printf("CHECKING ADJ\n");
	std::vector<Vertex<std::string>*> out;
	m_GraphV[v1]->getOutput(out);
	
	for(unsigned int i = 0; i < out.size(); i++){
		if(out[i]->getVertexID() == v2)
			return true;
	}

	
	return false;
	
}

bool Graph::isAdjacent(int v1, int v2){
	//printf("CHECKING ADJ betwee %d and %d\n", v1, v2);
	std::vector<Vertex<std::string>*> out;
	std::vector<Vertex<std::string>*> in;
	m_GraphV[v1]->getOutput(out);
	m_GraphV[v1]->getInput(in);
	

	//printf("Looking through V1's output\n");
	for(unsigned int i = 0; i < in.size(); i++){
		//printf("checking in %d\n", in[i]->getVertexID());
		if(in[i]->getVertexID() == v2)
			return true;
	}
	
	for(unsigned int i = 0; i < out.size(); i++){
		//printf("checking out %d\n", out[i]->getVertexID());
		if(out[i]->getVertexID() == v2)
			return true;
	}

	
	return false;
	
}

bool Graph::isConnected(bool isRoot){
	//printf("Checking to see if graph: %s is connected. Starting:  %d\n", m_Name.c_str(), m_GraphV.begin()->first);
	std::map<int, bool> found;
	DFS(m_GraphV.begin()->second, found);

	//printf("Graph numv: %d found: %d\n", getNumVertex(), found.size());
	if((getNumVertex()) == found.size()){
		//printf("CONNECTED\n");
		return true;
	}
	else{
		//printf(" NOT CONNECTED\n");
		if(!isRoot)
			return false;

	
		Graph* part1 = this;

		while(found.size() != part1->getNumVertex()){


			Graph* secondPart = new Graph("secondgraph");
			Graph* connectedPart = new Graph("connectedgraph");
			std::map<int, Vertex<std::string>*>::iterator it;
			for(it = part1->getBeginIterator(); it != part1->getEndIterator(); it++){
				Vertex<std::string>* tmp;
				tmp = it->second;
				it->second = NULL;
				if(found.find(it->first) == found.end())
					secondPart->addVertex(it->first, tmp);
				else
					connectedPart->addVertex(it->first, tmp);
			}
	
			//printf("PART\n");
			//secondPart->printg();
			//printf("connectd\n");
			//connectedPart->printg();
			m_ConnectedGraphs.push_back(connectedPart);
	
			found.clear();
			DFS(secondPart->getBeginIterator()->second, found);
			if(part1 != this)
				delete part1;

			part1 = secondPart;
			//printf("Graph numv: %d found: %d\n", part1->getNumVertex(), found.size());
		}

		m_ConnectedGraphs.push_back(part1);
		//printf("CONNECTED SIZE:%d\n",m_ConnectedGraphs.size());

		return false;
	}
		
}





void Graph::DFS(Vertex<std::string>* v, std::map<int, bool>& found, unsigned int terminate){

	std::vector<Vertex<std::string>*> out;
	std::vector<Vertex<std::string>*> in;
	v->getOutput(out); 
	v->getInput(in); 
	found[v->getVertexID()] = true;
	
	if(terminate == found.size())
		return;

	
	//Look at the output node
	for(unsigned int i = 0; i < out.size(); i++){
		//printf("DFS NODE: %d Out: %d\n", v->getVertexID(), out[i]->getVertexID());
		if(found.find(out[i]->getVertexID()) == found.end()){
			DFS(out[i], found, terminate);	
			if(terminate == found.size())
				return;
		}
	}
	
	//Look at the input node
	for(unsigned int i = 0; i < in.size(); i++){
		//printf("DFS NODE: %d In: %d\n", v->getVertexID(), in[i]->getVertexID());
		if(found.find(in[i]->getVertexID()) == found.end()){
			DFS(in[i], found, terminate);	
			if(terminate == found.size()){
				return;
			}
		}
	}
}

void Graph::DFS(Vertex<std::string>* v, std::map<int, bool>& found){
	std::vector<Vertex<std::string>*> out;
	std::vector<Vertex<std::string>*> in;
	v->getOutput(out); 
	v->getInput(in); 
	found[v->getVertexID()] = true;
	
	//Look at the output node
	for(unsigned int i = 0; i < out.size(); i++){
		//printf("DFS NODE: %d Out: %d\n", v->getVertexID(), out[i]->getVertexID());
		if(found.find(out[i]->getVertexID()) == found.end())
			DFS(out[i], found);	
	}
	
	//Look at the input node
	for(unsigned int i = 0; i < in.size(); i++){
		//printf("DFS NODE: %d In: %d\n", v->getVertexID(), in[i]->getVertexID());
		if(found.find(in[i]->getVertexID()) == found.end())
			DFS(in[i], found);	
	}

}

void Graph::DFS(Vertex<std::string>* v, std::map<int, bool>& found, std::set<int>& partition){
	std::vector<Vertex<std::string>*> out;
	std::vector<Vertex<std::string>*> in;
	v->getOutput(out); 
	v->getInput(in); 
	found[v->getVertexID()] = true;
	//printf("Vertex Traversed: %d\n", v->getVertexID());
	//printf("SIze of found %d\n", found.size());
	
	//Look at the output node
	for(unsigned int i = 0; i < out.size(); i++){
	//	printf("DFS NODE: %d Out: %d\n", v->getVertexID(), out[i]->getVertexID());
		if(partition.find(out[i]->getVertexID()) != partition.end()){
	//		printf("output is part of partition\n");
			if(found.find(out[i]->getVertexID()) == found.end()){
				DFS(out[i], found, partition);	
			}
		}
	}
	
	//Look at the input node
	for(unsigned int i = 0; i < in.size(); i++){
	//	printf("DFS NODE: %d In: %d\n", v->getVertexID(), in[i]->getVertexID());
		if(partition.find(in[i]->getVertexID()) != partition.end()){
	//		printf("input is part of partition\n");
			if(found.find(in[i]->getVertexID()) == found.end()){
				DFS(in[i], found, partition);	
			}
		}
	}
	//printf("DONE\n\n");
}


bool Graph::difference(std::vector<int>& subNodes, std::set<int>& diff){
	typename std::map<int, Vertex<std::string>*>::iterator it;
	std::set<int> subNodeSet;

	for(unsigned int i = 0; i < subNodes.size(); i++)
		subNodeSet.insert(subNodes[i]);
		
	//Copies all the vertices into diff
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		if(subNodeSet.find(it->first) == subNodeSet.end())
			diff.insert(it->first);
	}
	//printf("Difference graph\n");
	std::set<int>::iterator sit;
	//for(sit = diff.begin(); sit != diff.end(); sit++){
	//	printf("%d ", *sit);
	//}
	//printf("\n");
	
	std::map<int, bool> found;
	DFS(m_GraphV[*(diff.begin())], found, diff);
	//printf("FOUND: %d, DIFF: %d\n", found.size(), diff.size());
	if(found.size() == diff.size()){
		//printf("TRUE\n");
		return true;
	}
	else 
		return false; 
}

void Graph::resetStatus(){
	std::map<int, Vertex<std::string>*>::iterator it; 

	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		it->second->setStatus(0);
	}
}

void Graph::clearF(){
	for(unsigned int i = 0; i < F.size(); i++)
		delete F[i];
	F.clear();
}
/*
void printMahala(){
	typename std::map<int, Vertex<std::string>*>::iterator it;
	printf("node in out\n");
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		std::vector<int> in;
		it->second->getInput(in);
		std::vector<int> out;
		it->second->getOutput(out);
		printf("%d, %d;\n", in.size(), out.size());
	}

	printf("\n\n");
}	
void relabelVertex(int label, int newLabel){
	Vertex<std::string>* vertex= m_GraphV[label];
	vertex->setVertexID(newLabel);
	m_GraphV[newLabel] = vertex;
	m_GraphV.erase(label);
	vmap[0].push_back(newLabel);
	vmap[1].push_back(label);
}


void clearLevels(){
	typename std::map<int, Vertex<std::string>*>::iterator it;
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		it->second->setLevel(0);
	}
}

void isCyclic(){
	std::map<Vertex<std::string>*, bool> done;

	for(unsigned int i = 0; i < getNumVertex(); i++){
		done[m_GraphV[i]] = false;
	}

	for(unsigned int i = 0; i < getNumVertex(); i++){
		if(m_GraphV[i]->getLevel() == 0){
			DFS(m_GraphV[i], done);
		}
	}
}


void setLevels(){
	//Go through each input
		for(unsigned int i = 0; i < m_Inputs.size(); i++){
			std::vector<int> output;
			m_GraphV[m_Inputs[i]]->getOutput(output);
			printf("\nINPUT %d\n", m_Inputs[i]); 

			//Get the nodes the input is connected to. 	
			for(unsigned int i = 0; i < output.size(); i++){
				Vertex<std::string>* vertex = m_GraphV[output[i]];
				int currLevel = vertex->getLevel();
	
				printf("NODE: %d\tCURRENT LEVEL: %d\t NEW LEVEL: %d\n", output[i], vertex->getLevel(), 1);
				
				//Make sure the node isn't set already
				if(currLevel > 0)
					continue;
				else
					vertex->setLevel(1);
			}
	
			//Traverse through the circuit
			for(unsigned int i = 0; i < output.size(); i++){
				traverse(output[i], 2);
			}
		}

}

*/


std::string Graph::getSingleItem(){
	return m_GraphV.begin()->second->getType();
}

void Graph::remapVertex(int v, int newv){
	if(m_GraphV.size() != 1){
		printf("Cannot remap. Number of vertices in graph is not 1\n");
		exit(1);
	}
	Vertex<std::string>* tmpv = m_GraphV[v];
	m_GraphV.clear();
	m_GraphV[newv] = tmpv;	
	tmpv->setVertexID(newv);
}

void Graph::remapEdgeSet(int v, int newv){
	for(unsigned int i = 0; i < E.size(); i++){
		for(unsigned int j = 0; j < E[i]->size(); j++){
			if(E[i]->at(j) == v)
				E[i]->at(j) = newv; 
		}
	}

}

void Graph::remapVMap(int v, int newv){
		int vmap0;
		std::map<int,int>::iterator it = vmap.find(v);
		if(it != vmap.end()){
			vmap0 = it->second;
			vmap.erase(it);
			vmap[newv] = vmap0;
		}
		else
			vmap[newv] = v;


}




std::string Graph::getSingleType(){
	std::map<int, Vertex<std::string>*>::iterator it;
	it = m_GraphV.begin();

	return it->second->getType();
}



void Graph::resizeE(int size){
	E.reserve(size);
	//printf("E CAP: %d\n", E.capacity());
}

void Graph::resizeF(int size){
	//printf("OLD F CAP: %d\n", F.capacity());
	//printf("Fsize: %d\n", F.size());
	F.reserve(size);
	//printf("F CAP: %d\n", F.capacity());
}

unsigned int Graph::getNumEdges(){
	return m_NumEdges;
}

void Graph::setNumEdges(unsigned int edges){
	m_NumEdges = edges;
}
	



int Graph::getNumGate(std::string gate){
	return m_numType[gate].size();
}

void Graph::removeInputs(){
	//printf("GRAPH WITH INPUTS\n");
	//printg();
	std::map<int, Vertex<std::string>*>::iterator it;
	bool deleted = false;
	std::vector<int> toBeDeleted;
	
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		//printf("CHekcing node: %d\n", it->first);
		if(deleted){
			//it = m_GraphV.begin();
			deleted = false;
		}

		if(it->second->getType() == "IN"){
			//printf("INPUT V: %d\n", it->first);
			std::vector<Vertex<std::string>*> out;
			it->second->getOutput(out);

			//Disconnect the input from the nodes
			for(unsigned int i = 0; i < out.size(); i++){
				//printf("Output from IN: %d\n", out[i]->getVertexID());

				//if the only input is IN, remove the gate as well
				//if(out[i]->getIVSize() == 1){
				//	std::vector<Vertex<std::string>*> out2;
			//		out[i]->getOutput(out2);
		//			for(unsigned int j = 0; j < out2.size(); j++){
		//				//printf("Output from IN: %d\n", out[i]->getVertexID());
		//				out2[j]->removeInput(out[i]->getVertexID());
		//			}
		//			toBeDeleted.push_back(out[i]->getVertexID());
		//		}

				out[i]->removeInput(it->first);
					//printf("Input Removed\n");
				//else
				//	printf("INPUT NOT REMOVED**************\n");
			}

			//Remove the Input
			//m_GraphV.erase(it->first);
			toBeDeleted.push_back(it->first);
			deleted = true;
			//printf("\n\n");
		}
	}
	m_Inputs.clear();

	//printf("Removing Inputs from graph\n");
	for(unsigned int i = 0; i < toBeDeleted.size(); i++){
		delete m_GraphV[toBeDeleted[i]];
		m_GraphV.erase(toBeDeleted[i]);
	}
	//printf("Number of vertices: %d\n", getNumVertex());
	//printf("Number of nodes removed %d\n", toBeDeleted.size());
	//printg();
	
	
	int count = 0;
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		it->second->setVertexIndex(count);
		count++;
	}
}








void Graph::printV(std::string name, std::vector<int>* v){
	printf("%s\n", name.c_str());
	for(unsigned int i = 0; i < v->size(); i++){
		printf("%3d ", v->at(i));
	}
	printf("\n");
	
}
void Graph::printV(std::string name, std::vector<int> & v){
	printf("%s\n", name.c_str());
	for(unsigned int i = 0; i < v.size(); i++){
		printf("%3d ", v[i]);
	}
	printf("\n");
	
}
void Graph::print2DV(std::string name, std::vector<std::vector<int> > & v){
	printf("%s\n", name.c_str());
	for(unsigned int i = 0; i < v.size(); i++){
		for(unsigned int j = 0; j < v[i].size(); j++){
			printf("%3d ", v[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void Graph::print2DV(std::string name, std::vector<std::vector<int>* > & v){
	printf("%s\n", name.c_str());
	for(unsigned int i = 0; i < v.size(); i++){
		for(unsigned int j = 0; j < v[i]->size(); j++){
			printf("%3d ", v[i]->at(j));
		}
		printf("\n");
	}
	printf("\n");
}



void Graph::substitute(int node, Graph* sub){
	//printf("SUBSTITUTION!\n");
	typename std::map<int, Vertex<std::string>*>::iterator begin;
	typename std::map<int, Vertex<std::string>*>::iterator end;
	typename std::map<int, Vertex<std::string>*>::iterator it; 
	
	std::vector<Vertex<std::string>*> inputNode;
	std::vector<Vertex<std::string>*> outputNode;
	std::vector<int> subInputs;
	sub->getInputs(subInputs);

	m_GraphV[node]->getInput(inputNode);
	m_GraphV[node]->getOutput(outputNode);

/*	if(outputNode.size() > 1){
		printf("Unknown Substitution. More than one output.\nExiting\n");
		exit(1);
	}
	*/
	if(inputNode.size() != subInputs.size()){
		printf("Number of Inputs for substitution does not match.\nExiting\n");
		exit(1);
	}


	for(unsigned int i = 0; i < inputNode.size(); i++){
		//Remove node from the output of its inputs
		inputNode[i]->removeOutputValue(node);
		
		//Get all the output node from a input of sub
		std::vector<Vertex<std::string>*> inOutputs;
		sub->getVertex(subInputs[i])->getOutput(inOutputs);

		//For each output, add it to the output
		for(unsigned int j = 0; j < inOutputs.size(); j++){
			inputNode[i]->addOutput(inOutputs[j]);

			//Remove the original input of the currently added output
			inOutputs[j]->removeInputValue(subInputs[i]);
			inOutputs[j]->addInput(inputNode[i]);
		}
	}
	
		
	bool found = false;
	sub->getIterators(begin, end);
	for(it = begin; it != end; it++){
		if(it->second->getOVSize() == 0){
			found = true;
			break;
		}
	}

	if(!found){
		printf("ERROR: No output found in circuit\n");
		exit(1);
	}

	for(unsigned int i = 0; i < outputNode.size(); i++){
		//Remove node from the output of its inputs
		outputNode[i]->removeInputValue(node);
		outputNode[i]->addInput(it->second);
		it->second->addOutput(outputNode[i]);
		
		//Find the output node
		
	}


	//Add the new vertices into the graph
	sub->getIterators(begin, end);
	for(it = begin; it != end; it++){
		if(it->second->getType() != "IN"){
			addVertex(it->second->getVertexID(), it->second);
			it->second = NULL;
		}
	}

	delete sub;
	//printf("SUBSTITUTION COMPLETE\n");

}


void Graph::setLevels(){
	printf("Setting Levels\n");

	typename std::map<int, Vertex<std::string>*>::iterator it; 	
	std::queue<Vertex<std::string>*, std::list<Vertex<std::string>*> > queue;
	//printf("Getting Starting Inputs\n");
	for(unsigned int i = 0; i < m_Inputs.size(); i++){
		//printf("Pushing\tV%d\n", m_Inputs[i]);
		queue.push(m_GraphV[m_Inputs[i]]);
	}


	std::map<int,bool> mark;
	BFSLevel(queue, mark, 0, queue.size()-1);


}


void Graph::BFSLevel(std::queue<Vertex<std::string>*, std::list<Vertex<std::string>*> > queue, std::map<int,bool>& mark, int level, int endLevel){

		if(queue.empty())
			return;

		Vertex<std::string>* front;
		front = queue.front();
		queue.pop();

		if(mark.find(front->getVertexID()) == mark.end()){
			//printf("Node\t%d not marked. LEVEL: %d\n", front->getVertexID(), level);
			front->setLevel(level);
		}
		else{
			if(front->getLevel() < level)
				front->setLevel(level);
		}

		std::vector<Vertex<std::string>*> output;
		front->getOutput(output);

		for(unsigned int i = 0; i < output.size(); i++){
			queue.push(output[i]);
		}
		
		if(endLevel == 0){
			level++;
			endLevel = queue.size()-1;
		}
		else
			endLevel--;

		BFSLevel(queue, mark, level, endLevel);

}


int Graph::getMaxLevel(){
	return m_MaxLevel;
}
