#ifndef TYPEANALYSIS_H
#define TYPEANALYSIS_H
/////////////////////////////////////////////////////////////////////////////
// TypeAnalysis.h - Support file and directory operations                  //
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
This module defines the TypeAnalysis class.
This module contains the function declarations
used by to create the Type Analysis table
Public Interface:
=================
bool Analyze(std::string filename)       //creates the Type Table
void start()                             //starts multiple threads

*/


#include <string>
#include "../TypeTable/TypeTable.h"
#include "..//QueuedWorkItems/QueuedWorkItems.h"
class TypeAnalysis
{

private: 
	ProcessWorkItem<bool> *tp;
public :
	TypeAnalysis() {
		TypeTableAnalysis = new TableTree;
	}
	bool Analyze(std::string filename);
	void start(ProcessWorkItem<bool> *tp);
	void mergeTables();
	static TableTree* TypeTableAnalysis ;
};



#endif