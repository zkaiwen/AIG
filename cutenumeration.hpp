/*@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@
 @
 @      cutenumeration.hpp
 @      
 @      @AUTHOR:Kevin Zeng
 @      Copyright 2012 – 2013 Virginia Polytechnic Institute and State University
 @
 @#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@*/

#ifndef CUTENUMERATION_GUARD
#define CUTENUMERATION_GUARD

#include "aig.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <set>
#include <vector>


class CutEnumeration{
	private:
		std::vector<std::set<std::set<int> > > m_Cuts;
		AIG* m_AIGraph;
		

	public:
		CutEnumeration();
		~CutEnumeration();


		void findKFeasibleCuts(AIG*, int);
		void cut(int, int, std::set<std::set<int> >&);
		void merge(std::set<std::set<int> >&, std::set<std::set<int> >&, int);

};




#endif
