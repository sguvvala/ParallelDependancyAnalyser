#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H


/*
  Module Operations: 
  ==================
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
    parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv CodeAnalysis.sln
    - cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 2.1 : 15 Feb 16
  - small functional change to a few of the actions changes display strategy
  - preface the (new) Toker and SemiExp with Scanner namespace
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
    of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "ASTree.h"
#include "../TypeTable/TypeTable.h"
#include <unordered_map>
///////////////////////////////////////////////////////////////
// ScopeStack element is application specific
/* ToDo:
 * - chanage lineCount to two fields: lineCountStart and lineCountEnd
 */

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.
/*
 * ToDo:
 * - add AST Node class
 * - provide field to hold root of AST
 * - provide interface to access AST
 * - provide helper functions to build AST, perhaps in Tree class
 */
class Repository  // application specific
{
  ScopeStack<element*> stack;
  Scanner::Toker* p_Toker;
  std::string typeTableFileName;
  
public:

	TableTree* TableTreeObj;// = new TableTree;
	static std::unordered_map<std::string, Repository*> _instances;
	static Repository *_instance;
	AST *asTree = new AST();
  Repository(Scanner::Toker* pToker)
  {
    p_Toker = pToker;
	TableTreeObj = new TableTree;
	//_instance = this;
  }
  void setInstance(std::string fn, Repository *repo) {
	  _instances[fn] = repo;
  }
  Repository * getInstance(std::string fn) {
	  if (_instances.count(fn) == 0)
	  {
		  std::cout << "\n No Repository object found for this file.";
		  return nullptr;
	  }
	  return _instances[fn];
  }
  void setFileName(std::string name) {
	  typeTableFileName = name;
  }
  /*std::string getFileName() {
	  return typeTableFileName;
  }*/
  ScopeStack<element*>& scopeStack()
  {
    return stack;
  }
  Scanner::Toker* Toker()
  {
    return p_Toker;
  }
  size_t lineCount()
  {
	  return (size_t)(p_Toker->currentLineCount());
  }
  static Repository* getInstances()
  {
	  return _instance;
  }
  std::string& getTypeTableName()
  {
	  return typeTableFileName;
  }
  std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>>getTable()
  {
	  return TableTreeObj->Table;
  }

};

///////////////////////////////////////////////////////////////
// rule to detect type definitions

class TypeDef : public IRule {
public:
	
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("class") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		else if (pTc->find("struct") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		else if(pTc->find("enum") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		else if(pTc->find("namespace") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return false;
	}
};
///////////////////////////////////////////////////////////////
// action to handle type definitions
class PushTypes : public IAction
{

	Repository* p_Repos;
public:
	//TableTree* TableTreeObj = new TableTree;
	PushTypes(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	//return name of type
	std::string isType(const std::string& tok)
	{
	const static std::string keys[]
	= { "class", "struct", "namespace","enum" };
	for (int i = 0; i < 4; ++i)
		if (keys[i] == tok)
			return tok;
	return "";
	}
	void doAction(ITokCollection*& pTc)
	{
	//ConfigParseToConsole cpc;
		std::string type, name;
		if (p_Repos->scopeStack().size() == 0)
			return;
		element* elem = new element;
		ITokCollection& tc = *pTc;
		for (size_t i = 0; i < pTc->length(); i++) {
		if (isType((*pTc)[i]) != "")
		type = isType((*pTc)[i]);
		}
		size_t type_i = pTc->find(type);
		name = (*pTc)[type_i + 1];
		elem->type = type;
		elem->name = name;
		elem->startLineCount = p_Repos->lineCount();
		element *elem2 = p_Repos->scopeStack().top();
		elem2->_children.push_back(elem);
		//std::string names, filename;
		p_Repos->TableTreeObj->TypeTree = name; 
		p_Repos->TableTreeObj->TableType.push_back(std::make_pair(type, p_Repos->getTypeTableName()));
		p_Repos->TableTreeObj->Table.insert({ p_Repos->TableTreeObj->TypeTree,p_Repos->TableTreeObj->TableType });
		//p_Repos->TableTreeObj->showTypeTable();
		p_Repos->scopeStack().push(elem);
	}
	//p_Repos->TableTreeObj->showTypeTable();
};
///////////////////////////////////////////////////////////////
// action to show type definition

class ShowTypeDef : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		// remove comments
		Scanner::SemiExp se;
		for (size_t i = 0; i < tc.length(); ++i)
		{
			if (!se.isComment(tc[i]))
				se.push_back(tc[i]);
		}
		// show cleaned semiExp
		std::cout << "\n  Type Definition: " << se.show();
	}
};


///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("{") < pTc->length())
    {
		
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
  Repository* p_Repos;
public:
  HandlePush(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  bool isSpecialKeyWord(const std::string& tok)
  {
	  const static std::string keys[]
		  = { "for", "while", "switch", "if", "catch" };
	  for (int i = 0; i<5; ++i)
		  if (tok == keys[i])
			  return true;
	  return false;
  }
  void doAction(ITokCollection*& pTc)
  {
    //std::cout << "\n--BeginningOfScope rule";
	  
	  if (p_Repos->asTree->getRoot() == nullptr)
	  {
		  element *_elem = new element;
		  _elem->type = "GNS";
		  _elem->name = "GlobalNameSpace";
		  _elem->startLineCount = 0;
		  p_Repos->asTree->setRoot(_elem);
		  p_Repos->scopeStack().push(_elem);
	  }
	  ITokCollection& tc = *pTc;

	  
    element *elem = new element;
	size_t len = tc.find("(") - 1;
	if (isSpecialKeyWord(tc[len]))
	{
		if (tc[len] == "for")
		{
			elem->type = "for";
			elem->name = "for Loop";
		}
		if (tc[len] == "while")
		{
			elem->type = "while";
			elem->name = "while Loop";
		}
		if (tc[len] == "switch")
		{
			elem->type = "switch";
			elem->name = "switch case";
		}
		if (tc[len] == "if")
		{
			elem->type = "if";
			elem->name = "if statement";
		}
		if (tc[len] == "catch")
		{
			elem->type = "catch";
			elem->name = "catch block";
		}
	}
	else if (tc[tc.length() - 3] == "try")
	{
		elem->type = "try";
		elem->name = "try block";
	}
	else if (tc[tc.length() - 3] == "else")
	{
		elem->type = "else";
		elem->name = "else statement";
	}
	else
	{
		elem->type = "unknown";
		elem->name = "anonymous";
	}
    elem->startLineCount = p_Repos->lineCount();
	element  * elem2 = p_Repos->scopeStack().top();
	elem2->_children.push_back(elem);
	p_Repos->scopeStack().push(elem);
  }
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
  Repository* p_Repos;
public:
  HandlePop(Repository* pRepos)
  {
    p_Repos = pRepos;
  }

  void doAction(ITokCollection*& pTc)
  {
    //std::cout << "\n--EndOfScope rule";
    if(p_Repos->scopeStack().size() == 0)
      return;
    element *elem = p_Repos->scopeStack().pop();
	elem->endLineCount = p_Repos->lineCount() + 1;

    if(elem->type == "function")
    {
      //std::cout << "\nHandlePop";
      //std::cout << "\n--popping at line count = " << p_Repos->lineCount();
    //  std::cout << "\n  Function " << elem->name << ", lines = " << p_Repos->lineCount() - elem->startLineCount + 1;
     // std::cout << "\n";
    }
  }
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("#") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
	   std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
  }
};

class LambdaDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find_frmEnd("]");
			//size_t len2 = tc.find("(")
			if ((len < tc.length() && tc[len+1] == "(" )|| tc[tc.length() - 2] == "]")
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

class PushLambda : public IAction
{
	Repository* p_Repos;
public:
	PushLambda(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// next statement is now  handled in PrintFunction
		// std::cout << "\n  FunctionDef: " << pTc->show();
		// pop anonymous scope
		p_Repos->scopeStack().pop();
		element *elem2 = p_Repos->scopeStack().top();
		elem2->_children.pop_back();

		// push function scope
		// name = (*pTc)[pTc->find("(") - 1];
		element *elem = new element;
		elem->type = "lambda";
		//elem->name = name;
		elem->startLineCount = p_Repos->lineCount();
		//element *elem1;
		elem2 = p_Repos->scopeStack().top();
		elem2->_children.push_back(elem);
		p_Repos->scopeStack().push(elem);
	}
};

class PrintLambda : public IAction
{
	Repository* p_Repos;
public:
	PrintLambda(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		size_t len = tc.find_frmEnd("[");
		std::cout << "\n  Executable: \n  ";
		for (size_t i = 0; i < len - 2; i++)
			if (tc[i] != "\n")
				std::cout << tc[i];
		std::cout << "\n";
		std::cout << "\n  Lambda: \n  "  ;//<< pTc->show().c_str();
		for (size_t i = len-1; i < tc.length(); i++)
			if (tc[i] != "\n")
				std::cout << tc[i];
		std::cout << "\n";
	}
};


class PrintClass : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
			std::cout << "\n\n  Class Defn: " << pTc->show().c_str();
	}
};


///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)  //class , struct namespace, enum
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for(int i=0; i<5; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{")
    {

      size_t len = tc.find("(");
	  size_t len2 = tc.find_frmEnd("]");
	  bool isLambda = (len2 < tc.length() && tc[len2 + 1] == "(") || tc[tc.length() - 2] == "]";
      if(len < tc.length() && !isSpecialKeyWord(tc[len-1]) && !isLambda)
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // next statement is now  handled in PrintFunction
    // std::cout << "\n  FunctionDef: " << pTc->show();

    // pop anonymous scope
    p_Repos->scopeStack().pop();
	element *elem2 =	p_Repos->scopeStack().top();
	elem2->_children.pop_back();

    // push function scope
    std::string name = (*pTc)[pTc->find("(") - 1];
    element *elem = new element;
    elem->type = "function";
    elem->name = name;
    elem->startLineCount = p_Repos->lineCount();
	//element *elem1;
	elem2 = p_Repos->scopeStack().top();
	elem2->_children.push_back(elem);
    p_Repos->scopeStack().push(elem);
	p_Repos->TableTreeObj->TypeTree = name;
	p_Repos->TableTreeObj->TableType.push_back(std::make_pair(elem->type, p_Repos->getTypeTableName()));
	p_Repos->TableTreeObj->Table.insert({ p_Repos->TableTreeObj->TypeTree,p_Repos->TableTreeObj->TableType });
	//p_Repos->TableTreeObj->showTypeTable();
	p_Repos->scopeStack().push(elem);
  }
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
  Repository* p_Repos;
public:
  PrintFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n  FuncDef: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    size_t len = pTc->find(")");
    std::cout << "\n\n  Pretty Stmt:    ";
    for(size_t i=0; i<len+1; ++i)
      std::cout << (*pTc)[i] << " ";
    std::cout << "\n";
  }
};

///////////////////////////////////////////////////////////////
// rule to detect declaration

class Declaration : public IRule          // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 22;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end + 1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
    //std::cout << "\n  -- " << tc.show();
  }
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i<5; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    //std::cout << "\n  -- " << tc.show();
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);

    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      //std::cout << "\n  ++ " << tc.show();
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);

      // remove modifiers, comments, newlines, returns, and initializers
      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i)
      {
        if (isModifier(tc[i]))
          continue;
        if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
          continue;
        if (tc[i] == "=" || tc[i] == ";")
          break;
        else
          se.push_back(tc[i]);
      }
      //std::cout << "\n  ** " << se.show();
      if (se.length() == 2)  // type & name, so declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to show declaration

class ShowDeclaration : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i<tc.length(); ++i)
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    // show cleaned semiExp
    std::cout << "\n  Declaration: " << se.show();
  }
};

///////////////////////////////////////////////////////////////
// rule to detect executable

class Executable : public IRule           // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 22;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end+1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end+1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
    //std::cout << "\n  -- " << tc.show();
  }
  
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i<5; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    //std::cout << "\n  -- " << tc.show();
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);

    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      //std::cout << "\n  ++ " << tc.show();
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);

      // remove modifiers, comments, newlines, returns, and initializers

      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i)
      {
        if (isModifier(tc[i]))
          continue;
        if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
          continue;
        if (tc[i] == "=" || tc[i] == ";")
          break;
        else
          se.push_back(tc[i]);
      }
      //std::cout << "\n  ** " << se.show();
      if (se.length() != 2)  // not a declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to show executable

class ShowExecutable : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i < tc.length(); ++i)
    {
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    }
    // show cleaned semiExp
    std::cout << "\n  Executable: " << se.show();
  }
};


#endif
