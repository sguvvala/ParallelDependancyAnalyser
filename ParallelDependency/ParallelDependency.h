#ifndef PARALLELDEPENDENCYANALYSIS_H
#define PARALLELDEPENDENCYANALYSIS_H

/////////////////////////////////////////////////////////////////////////////
// ParallelDependency.h - Support file and directory operations          //
// ver 1.0                                                                 //
//                                                                         //
// Language:    Visual C++, 2011                                           //
// Platform:    MACBOOK PRO                                                //
// Author:      Sruthi Guvvala, MS Computer Science                        //
//              Syracuse University                                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
/*
Module Operations:
=================
This module defines the ParallelDependencyAnalysis class.
used to run dependancy analysis on multiple threads
Public Interface:
=================
void start()
*/
#include <string>
#include "../TypeTable/TypeTable.h"
#include "../DependancyAnalysis/DependancyAnalysis.h"
class ParallelDependencyAnalysis
{
public:
	/*TypeAnalysis() {
		TypeTableAnalysis = new TableTree;
	}*/
	//bool Analyze(std::string filename);
	void start();
	//void mergeTables();
	//static TableTree* TypeTableAnalysis;
};



#endif