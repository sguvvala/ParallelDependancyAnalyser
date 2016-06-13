
/////////////////////////////////////////////////////////////////////////////
// FileSystem.cpp - Support file and directory operations                  //
// ver 1.0                                                                 //
//                                                                         //
// Language:    Visual C++, 2011                                           //
// Platform:    MACBOOK PRO                                                //
// Author:      Sruthi Guvvala, MS Computer Science                        //
//              Syracuse University                                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
/*
Module Operations :
== == == == == == == == =
This module defines the Abstract Syntax Tree.
Defines the structure of an element.
                         
*/


#include<vector>
#include <sstream>
#include <unordered_map>

struct element
{// DEfining the element
	std::string type;
	std::string name;
	size_t startLineCount = 0;
	size_t endLineCount = 0;
	size_t count = 0;
	std::string show()
	{//Defining the tree display 
		std::ostringstream temp;
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << "Start = ";
		temp << startLineCount;
		temp << ", ";
		temp << "End = ";
		temp << endLineCount;
		temp << ", ";
		temp << "Complexity = ";
		temp << count;
		temp << ")";
		return temp.str();
	}
	std::vector<element*> _children;
};
/*struct TableTree
{
	std::string TypeTree;
	std::vector<std::pair<std::string, std::string>>TableType;
	std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>>Table;
	void showTypeTable()
	{

		for (size_t i = 0; i < TableType.size(); i++)
		{
			std::cout << "type is : " << TypeTree << "\t";
			std::cout << "name is : " << TableType.at(i).first << "\t";
			std::cout << "Filename is : " << TableType.at(i).second << "\n";
		}
	}

};*/
