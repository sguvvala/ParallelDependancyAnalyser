#ifndef ITOKCOLLECTION_H
#define ITOKCOLLECTION_H
/////////////////////////////////////////////////////////////////////
//  ITokCollection.h - package for the ITokCollection interface    //
//  ver 1.2                                                        //
//                                                                 //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        James Fawcett                                   //
//                 Syracuse University                                                                        //
//  ver 1.3                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Sruthi Guvvala, MS Computer Science             //
//                 Syracuse University                             //
/////////////////////////////////////////////////////////////////////
/*
  Module Purpose:
  ===============
  ITokCollection is an interface that supports substitution of different
  types of scanners for parsing.  In this solution, we illustrate that
  by binding two different types of collections, SemiExp and XmlParts,
  to this interface.  This is illustrated in the test stubs for the
  SemiExpression and XmlElementParts modules.

  Maintenance History:
  ====================
  ver 1.2 : 15 Feb 2016
  - removed merge, added non-const operator[] overload
  ver 1.1 : 02 Jun 11
  - added merge, remove overload, and default param in get
  ver 1.0 : 17 Jan 09
  - first release
*/
#include <string> 

struct ITokCollection
{
  virtual bool get(bool clear=true)=0;
  virtual size_t length()=0;
  virtual std::string& operator[](size_t n) = 0;
  virtual std::string operator[](size_t n) const = 0;
  virtual size_t find(const std::string& tok)=0;
  virtual size_t find_frmEnd(const std::string& tok) = 0;
  virtual void push_back(const std::string& tok)=0;
  virtual bool remove(const std::string& tok)=0;
  virtual bool remove(size_t i)=0;
  virtual void toLower()=0;
  virtual void trimFront()=0;
  virtual void clear()=0;
  virtual std::string show(bool showNewLines=false)=0;
  virtual bool isComment(const std::string& tok) = 0;
  virtual ~ITokCollection() {};
};

#endif