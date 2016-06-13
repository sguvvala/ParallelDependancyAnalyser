/////////////////////////////////////////////////////////////////////////////
// ParallelDependency.cpp - Support file and directory operations          //
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
#include "ParallelDependency.h"
#include "..//TypeAnalysis/TypeAnalysis.h"
#include "..//DependancyAnalysis/DependancyAnalysis.h"
#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "..//QueuedWorkItems/QueuedWorkItems.h"
#include "..//FileMgr/FileMgr.h"
#include "..//FileMgr/FileSystem.h"

void ParallelDependencyAnalysis::start()// To start multiple threads
{
	ProcessWorkItem<bool> tp(8);
	tp.start();
	//get vector of file
	std::string path1 = "C:\\Users\\Sruthi Guvvala\\Desktop\\Project3\\Parser";
	std::vector<std::string> a1 = FileSystem::Directory::getFiles(path1, "*.h");
	//std::cout << "tp started : " << a1.size();
	std::vector<std::string>::iterator iter1;
	WorkItem<bool> wi;
	DependancyAnalysis *da = new DependancyAnalysis;
	for (iter1 = a1.begin(); iter1 != a1.end(); iter1++) {
		std::string filename = path1 + "\\" + *iter1;
		wi = [=]() {
			return da->findDep(filename);
		};
		WorkItem<bool> *wi1 = &wi;
		tp.doWork(wi1);
	}
	//after loop over
	tp.doWork(nullptr);
	tp.wait();
	std::unordered_map<std::string, Repository*>::iterator it;
	std::cout << "------------------------------------------------------------------------------------\n";
}

#ifdef TEST_PARALLELDEPENDENCYANALYSIS

int main() {
	ProcessWorkItem<bool> *tp = new ProcessWorkItem<bool>(8);
	TypeAnalysis ta;
	ta.start(tp);
	ParallelDependencyAnalysis pda;
	pda.start();
	return 0;
}

#endif