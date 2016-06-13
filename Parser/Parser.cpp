/////////////////////////////////////////////////////////////////////
//  Parser.cpp - Analyzes C++ language constructs                  //
//  ver 1.6                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Sruthi Guvvala, MS Computer Science             //
//                 Syracuse University                             //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;
using namespace Utilities;

//----< register parsing rule >--------------------------------

void Parser::addRule(IRule* pRule)
{
  rules.push_back(pRule);
}
//----< get next ITokCollection >------------------------------

bool Parser::next() 
{ 
  // default operation of doFold(pTokColl) is to simply return
  // - code analysis provides specific rules for handling
  //   for statements and operator expressions which are
  //   bound in ConfigureParser.cpp

  bool succeeded = pTokColl->get();
  if(!succeeded)
    return false;
  return true;
}

//----< parse the SemiExp by applying all rules to it >--------

bool Parser::parse()
{
  bool succeeded = false;
  for(size_t i=0; i<rules.size(); ++i)
  {
    if(rules[i]->doTest(pTokColl))
      succeeded = true;
  }
  return succeeded;
}
//----< register action with a rule >--------------------------

void IRule::addAction(IAction *pAction)
{
  actions.push_back(pAction);
}
//----< invoke all actions associated with a rule >------------

void IRule::doActions(ITokCollection*& pTokColl)
{
  if(actions.size() > 0)
    for(size_t i=0; i<actions.size(); ++i)
      actions[i]->doAction(pTokColl);
	
}

//----< test stub >--------------------------------------------

#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"

#ifdef TEST_PARSER

#include <queue>
#include <string>
#define Util StringHelper
int main(int argc, char* argv[])
{
	Util::Title("Testing Parser Class", '=');
	putline();
	// collecting tokens from files, named on the command lines
	std::string path = "C:\\Users\\Sruthi Guvvala\\Desktop\\Project2HelperCode\\Project2HelperCode\\Parser";
	std::vector<std::string> a = FileSystem::Directory::getFiles(path, "*.cpp");
	std::vector<std::string>::iterator iter = a.begin();
	//std::cout << *iter;
	while (iter != a.end())
	{
		std::string fileSpec = path + "\\" + *iter;
		std::cout << fileSpec;
		std::string msg = "Processing file" + fileSpec;
		Util::title(msg);
		putline();
		ConfigParseToConsole configure;
		Parser* pParser = configure.Build();
		try
		{	if(pParser)
		{
		if(!configure.Attach(fileSpec))
		{
		std::cout << "\n  could not open file " << fileSpec << std::endl;
		continue;
		}}
		else
		{
		std::cout << "\n\n  Parser not built\n\n";
		return 1;
		}
		while (pParser->next())
		{
		pParser->parse();
		}
		std::cout << "\n";
		configure.printTree();
		}
		catch(std::exception& ex)
		{
		std::cout << "\n\n    " << ex.what() << "\n\n";
		}iter++;
		std::cout << "\n";
		}
	}
#endif