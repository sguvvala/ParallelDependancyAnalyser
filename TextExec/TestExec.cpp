
/////////////////////////////////////////////////////////////////////////////
// TestExec.cpp - automated unit test suit demonstrating                   //
//                all the requirements of this project                     //
// ver 1.0                                                                 //
//                                                                         //
// Language:    Visual C++, 2011                                           //
// Platform:    MACBOOK PRO                                                //
// Author:      Sruthi Guvvala, MS Computer Science                        //
//              Syracuse University                                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include "../FileMgr/FileMgr.h"
#include "..\\FileMgr\FileSystem.h"
#include "../QueuedWorkItems/QueuedWorkItems.h"
#include "../ParallelDependency/ParallelDependency.h"
#include "../TypeAnalysis/TypeAnalysis.h"
#include "../DependancyAnalysis/DependancyAnalysis.h"
#include <queue>
#include <string>
#include <unordered_set>
#define Util StringHelper

using namespace Scanner;
using namespace Utilities;
std::vector<std::string> getFiles(std::string dir, std::vector<std::string> patterns)
{
	DataStore ds;
	std::vector<std::string> files;
	FileMgr fm(dir, ds);
	for (std::string s : patterns)
		fm.addPattern(s);
	fm.search();
	for (auto fs : ds)
		files.push_back(fs);
	return files;
}

int main(int argc, char* argv[])
{
	std::cout << "-----Requirement 3 : Provided all the packages as described in the Purpose section-------\n\n";
	std::cout << "Requirement 4 : Provided a ThreadPool package executes enqueued work items asynchronously\n\n";

	std::vector<std::string> patterns = {"*.cpp","*.h"};
	std::vector<std::string> files = getFiles("./", patterns);
	//std::cout << files.size() << std::endl;
	for (auto it : files)
		std::cout << std::endl << it;
	ProcessWorkItem<bool> *tp = new ProcessWorkItem<bool>(8);
	TypeAnalysis ta;
	ta.start(tp);
	ParallelDependencyAnalysis pda;
	std::cout << "------------Requirement 7 and 8 : Dependancy Analysis and Parallel Dependency Analysis-----------\n";
	pda.start();
	return 0;


	
}
