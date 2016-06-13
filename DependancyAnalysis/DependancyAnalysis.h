#ifndef DEPENDANCYANALYSIS_H
#define DEPENDANCYANALYSIS_H
/////////////////////////////////////////////////////////////////////////////
// DependancyAnalysis.h - Support file and directory operations                     //
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
This module defines the DependancyAnalysis class.
This module contains the function declarations 
used by to create the Dependancy Analysis table
Public Interface:
=================
bool findDep(std::string filename)       //creates a table showing file dependency

*/

#include <string>
#include "../TypeTable/TypeTable.h"
class DependancyAnalysis
{
public:
	//std::vector<std::string> DependancyVector(Scanner::Toker* pToker);
	//TypeAnalysis() {
		//TypeTableAnalysis = new TableTree;
	//}
	bool findDep(std::string filename);
	//void start();
//	void mergeTables();
	//static TableTree* TypeTableAnalysis;
};



#endif



//#ifndef DEPENDANCYANALYSIS_H
//#define DEPENDANCYANALYSIS_H
/////////////////////////////////////////////////////////////////////////////
// TypeTable.h - Support file and directory operations                  //
// ver 1.0                                                                 //
//                                                                         //
// Language:    Visual C++, 2011                                           //
// Platform:    MACBOOK PRO                                                //
// Author:      Sruthi Guvvala, MS Computer Science                        //
//              Syracuse University                                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
/*#include<vector>
#include <sstream>
#include<unordered_map>
class DependancyAnalysis
{
	/*TableTree* DepenTable = new TableTree;
	std::vector<std::string> DependancyVector(Scanner::Toker* pToker)
	{
		//if(pToker)
	//}*/
/* public :
	bool findDep(std::string filename); // write imp in cpp
	void start();
	void mergeTables();

};*/
//#endif