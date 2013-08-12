/*@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@
 @
 @      aig.hpp
 @      
 @      @AUTHOR:Kevin Zeng
 @      Copyright 2012 – 2013 Virginia Polytechnic Institute and State University
 @
 @#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@*/

#ifndef AIG_GUARD
#define AIG_GUARD

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <fstream>
#include "graph.hpp"

//Use to compare keys for "HASH TABLE"
struct cmp_intp
{
   bool operator()(int const *a, int const *b)
   {
			//printf("A: %d B:%d\nA:%d B:%d\n", a[0], b[0], a[1], b[1]);
			if(b[0] < a[0] )
				return true;
			
			if(b[1] < a[1])
				return true;
	
			return false;
   }
};

class AIG{
	private:
		std::vector<int*> m_AIG;
		std::vector<int> m_Outputs;
		void importAIG(const char*, std::vector<std::vector<int>*>&);
		std::map<int*, int, cmp_intp> m_HashTable;
		
	public:
		AIG();
		~AIG();

		void importAIG(std::string);
		void convertGraph2AIG(Graph*);

		//Getters
		void getAIG(std::vector<std::vector<int>*>&);
		void getOutputs(std::vector<int>&);


		int create_and2(int, int);
		int create_input();
		int new_and_vertex(int,int);


		void print();
		void printHash();

};



#endif
