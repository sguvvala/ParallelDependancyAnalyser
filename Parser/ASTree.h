
	/////////////////////////////////////////////////////////////////////////////
	// ASTree.h - Tree operations                                              //
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
	This module defines the functions to run against the
	Abstract Syntax Tree.
	Defines the Abstract Syntax Tree class .

	Public Interface
	================
	element* getRoot()                     //returns the root
	void setRoot(element  *elem)           //sets the root
	AST* getTree()                         //returns the tree
	void TreeWalk(element* pItem)          //walks through the tree

	*/


#include "ASTNode.h"
#include<iostream>
class AST
{
	AST* tree;
	std::string filename;
public:
	element* getRoot() { return root; }
	void setRoot(element  *elem) { root = elem; }
	element *root = nullptr;
	AST* getTree() { return tree; };
	//Function to walk through the tree
	void TreeWalk(element* pItem)
	{
		static size_t indentLevel = 0;
		std::cout << "\n  " << std::string(2 * indentLevel, ' ') << pItem->show();
		auto iter = pItem->_children.begin();
		++indentLevel;
		//Iterating through every element recursively
		while (iter != pItem->_children.end())
		{
			TreeWalk(*iter);
			++iter;
		}
		--indentLevel;
	}
	// To get the descendants of an element abd its complexity
	size_t getDescendantsSize(element* node)
	{
		static size_t indentLevel = 0;
		size_t count = 0;
		std::vector<element*>::const_iterator cit;
		if(node != nullptr)
			//Iterating through every element in the tree
			for (cit = node->_children.begin(); cit != node->_children.end(); cit++)
			{
				count++;
				count += getDescendantsSize(*cit);
				node->count = count;
			}
		return count;
	}
};