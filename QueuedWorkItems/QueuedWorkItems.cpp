///////////////////////////////////////////////////////////////////////
// QueuedWorkItems.cpp - child thread processes enqueued work items  //
// Version 1.0                                                       //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016         //
// ver 2.0                                                           //
// Language:    Visual C++, 2011                                     //
// Platform:    MACBOOK PRO                                          //
// Author:      Sruthi Guvvala, MS Computer Science                  //
//              Syracuse University									 //
///////////////////////////////////////////////////////////////////////
/*
 * A single child thread processes work items equeued by main thread
 */

#include <string>
#include <iostream>
#include <conio.h>
#include "QueuedWorkItems.h"
#include "../Utilities/Utilities.h"

//----< demonstrate ProcessWorkItem class >----------------------------

using Util = Utilities::StringHelper;
using WorkResult = std::string;


#ifdef TEST_QWI
int main()
{
	Util::Title("Enqueued Work Items");
	ProcessWorkItem<WorkResult> pwi(3);
	std::cout << "\n  main thread id = " << std::this_thread::get_id();
	pwi.start();
	// define 1st work item
	WorkItem<WorkResult> wi1 = []() {
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		return "Hello from wi1";
	};
	pwi.doWork(&wi1);
	// define 2nd work item
	WorkItem<WorkResult> wi2 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		return "wi2 result = " + Utilities::Converter<size_t>::toString(sum);
	};
	pwi.doWork(&wi2);
	// the following calls to result() block until results are enqueued
	std::cout << "\n  " << pwi.result();
	std::cout << "\n  " << pwi.result();
	pwi.doWork(nullptr);
	// wait for child thread to complete
	pwi.wait();
}

#endif