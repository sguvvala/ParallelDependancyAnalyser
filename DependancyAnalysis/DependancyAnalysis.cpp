/////////////////////////////////////////////////////////////////////
//  DependancyAnalysis.cpp - implements new parsing rules and actions //
//  ver 2.1                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Sruthi Guvvala, MS Computer Science             //
//                 Syracuse University                             //
/////////////////////////////////////////////////////////////////////

/*
Module Operations:
=================
This module defines the DependancyAnalysis class.
This module contains the function declarations
used by to create the Dependancy Analysis table
Public Interface:
=================
bool findDep(std::string filename)       

*/

#include "DependancyAnalysis.h"
#include <string>
#include <mutex>
#include<unordered_set>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../TypeAnalysis/TypeAnalysis.h"
#include "..//QueuedWorkItems/QueuedWorkItems.h"
#include "..//FileMgr/FileMgr.h"
#include "..//FileMgr/FileSystem.h"
using namespace Scanner;
//creates a table showing file dependency
bool DependancyAnalysis::findDep(std::string filename)
{static std::mutex _mtx;
	std::lock_guard<std::mutex> l(_mtx);
	std::unordered_set<std::string> DependancyVector;
	std::string iter;
		std::vector<std::string> v;
		std::string::size_type i = 0;
		std::string::size_type j = filename.find('\\');
		while (j != std::string::npos)
		{v.push_back(filename.substr(i, j - i));
			i = ++j;j = filename.find('\\', j);
			if (j == std::string::npos)
			v.push_back(filename.substr(i, filename.length()));
		}iter= v[v.size() - 1];
	try{std::ifstream in(filename);
		if (!in.good())
		{std::cout << "\n  can't open " << filename << "\n\n";
			return 1;}
		Toker toker;
		toker.attach(&in);
		do
		{std::string tok = toker.getTok();
			if (tok == "\n")
				continue;
			else
			{std::string depenName;
				auto search = TypeAnalysis::TypeTableAnalysis->Table.find(tok);
				if (search != TypeAnalysis::TypeTableAnalysis->Table.end())
				{for (size_t i = 0; i < search->second.size(); i++)
					{depenName = search->second[i].second;
						if (FileSystem::Path::getName(depenName) != FileSystem::Path::getName(iter))
							DependancyVector.insert(depenName);//Contains all the files its dependant on 
					}}}} while (in.good());
			std::cout << std::endl;
			if (DependancyVector.empty())
				std::cout << iter << "  Independant";
			else
			{std::cout << iter << " depends on :";
				for (auto it : DependancyVector)
				{std::cout << "  ";
					std::cout << it;
				}}}
	catch (std::logic_error& ex)
	{std::cout << "\n  " << ex.what();}
	std::cout << "\n\n";//here
	return false;
}
#ifdef TEST_DEPENDANCYANALYSIS

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
int main() {
	TypeAnalysis ta;
	ta.start();
	std::vector<std::string> patterns = { "*.cpp","*.h" };
	std::vector<std::string> files = getFiles("C:\\Users\\Sruthi Guvvala\\Desktop\\Project3\\Parser\\", patterns);
	std::cout << files.size() << std::endl;
	for (auto it : files) {
		DependancyAnalysis da;
		da.findDep(it);
	}
	return 0;
}

#endif