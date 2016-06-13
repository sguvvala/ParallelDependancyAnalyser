/////////////////////////////////////////////////////////////////////////////
// TypeAnalysis.cpp - Support file and directory operations                     //
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
bool Analyze(std::string filename)       
void start()                             
void mergeTables()                      
*/


#include "TypeAnalysis.h"
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

TableTree* TypeAnalysis::TypeTableAnalysis = nullptr;
//creates the Type Table
bool TypeAnalysis::Analyze(std::string filename)
{
	std::cout << "\nIn analyze :" << filename<<std::endl;
	ConfigParseToConsole configure;
	Parser* pParser = configure.Build();
	try
	{
		if (pParser)
		{
			if (!configure.Attach(filename))
			{
				std::cout << "\n  could not open file " << filename << std::endl;
				return false;
			}}
		else
		{
			std::cout << "\n\n  Parser not built\n\n";
			return 1;
		}
		while (pParser->next())
		{
			pParser->parse();
		}}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	std::cout << "\n";
	return true;
}
// runs the threads 
void TypeAnalysis::start(ProcessWorkItem<bool> *tp) // dir,pattern,number of threads
{
	tp->start();
	//get vector of file
	std::string path1 = "C:\\Users\\Sruthi Guvvala\\Desktop\\Project3\\Parser";
	std::vector<std::string> a1 = FileSystem::Directory::getFiles(path1, "*.h");
	std::vector<std::string>::iterator iter1;
	WorkItem<bool> wi;
	for (iter1 = a1.begin(); iter1 != a1.end(); iter1++) {
		std::string filename = path1 + "\\" + *iter1;
		wi = [=]() {
			return this->Analyze(filename);
		};
		WorkItem<bool> *wi1 = &wi;
		tp->doWork(wi1);
	}
	//after loop over
	tp->doWork(nullptr);
	tp->wait();
	std::unordered_map<std::string, Repository*>::iterator it;
	std::cout << "\n------------------------------Requirement 6: Type Analysis-------------------------------\n";
	//call merge function
	mergeTables();
}
// merges all the Type Table
void TypeAnalysis::mergeTables()
{
	Repository::_instances;
	std::unordered_map<std::string, Repository*>::iterator it;
	for (it = Repository::_instances.begin(); it != Repository::_instances.end(); ++it)
	{
		std::cout << " FileName : " << it->first << "\t";
		std::cout << std::endl;
		Repository * repo = it->second;
		it->second->TableTreeObj->showTypeTable();
		TypeTableAnalysis->Table.insert(it->second->TableTreeObj->Table.begin(), it->second->TableTreeObj->Table.end());
	}
	std::cout << "--------------------------Complete Type Table------------------------------------------------------\n-";
	TypeTableAnalysis->showTypeTable();
}


#ifdef TEST_TYPEANALYSIS

int main() {
	TypeAnalysis ta;
	return 0;
}

#endif