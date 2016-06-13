#ifndef TYPETABLE_H
#define TYPETABLE_H
/////////////////////////////////////////////////////////////////////////////
// TypeTable.h - Support file and directory operations                     //
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
== == == == == == == == ==
This module defines a Table Tree class.
It defines the structure of the Table Tree 
which is the structure of the type table

Public Interface:
=================
void showTypeTable()                  //prints the type table
void concat(TableTree* Partial)      //mergers two type table 
*/

#include<vector>
#include<iostream>
#include <sstream>
#include<unordered_map>
//#include "../Parser/ConfigureParser.h"
struct TableTree
{
	std::string TypeTree;
	std::vector<std::pair<std::string, std::string>>TableType;
	std::unordered_map< std::string, std::vector<std::pair<std::string, std::string>>> Table;
 
	 void showTypeTable()
	 {
		 std::string type, filename;
		 //std::cout << " ----------------";// << it->first;
		 std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>>::iterator it;
		 for (it = Table.begin(); it != Table.end(); ++it)
		 {
			 std::cout << " Name : " << it->first << "\t";
			 for (size_t i = 0; i < it->second.size(); i++)
			 {
				 type = it->second.at(i).first;

				 filename = it->second.at(i).second;
			 }
			 std::cout << "Type : " << type << "\t";
			 std::cout << "File Name : " << filename << "\n";
		 }
		 std::cout << std::endl;

		 /*for (size_t i = 0; i < TableType.size(); i++)
		 {
			 std::cout << "type is : " << TypeTree << "\t";
			 std::cout << "name is : " << TableType.at(i).first << "\t";
			 std::cout << "Filename is : " << TableType.at(i).second << "\n";
		 }*/
	 }
		void concat(TableTree* Partial)
		{
			std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>>::iterator it;
			for (it = Partial->Table.begin(); it != Partial->Table.end();it++)
			{
				Table[it->first] = it->second;
			}
		}
		
};
#endif
