#ifndef QWI_H
#define QWI_H
///////////////////////////////////////////////////////////////////////
// QueuedWorkItems.h - child thread processes enqueued work items    //
// Version 1.0                                                       //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016         //
// ver 2.0                                                           //
// Language:    Visual C++, 2011                                     //
// Platform:    MACBOOK PRO                                          //
// Author:      Sruthi Guvvala, MS Computer Science                  //
//              Syracuse University                                  //
///////////////////////////////////////////////////////////////////////
/*
* A single child thread processes work items equeued by main thread
*/

#include <thread>
#include <functional>
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"

template<typename Result>
using WorkItem = std::function<Result()>;

///////////////////////////////////////////////////////////////////////
// class to process work items
// - each work item is processed sequentially on a single child thread

template<typename Result>
class ProcessWorkItem
{
public:
	ProcessWorkItem(size_t c=1) {
		tCount = c;
	}
  void start();
  void doWork(WorkItem<Result>* pWi);
  Result result();
  void wait();
  ~ProcessWorkItem();
private:
	size_t tCount;
  std::vector<std::thread*> _pThreads;
  BlockingQueue<WorkItem<Result>*> _workItemQueue;
  BlockingQueue<Result> _resultsQueue;
};
//----< wait for child thread to terminate >---------------------------

template<typename Result>
void ProcessWorkItem<Result>::wait()
{
	//std::cout << "Wait : " << _pThreads.size() << std::endl;
	for(auto thread : _pThreads)
		thread->join();
}
//----< enqueue work item >--------------------------------------------

template<typename Result>
void ProcessWorkItem<Result>::doWork(WorkItem<Result>* pWi)
{
  _workItemQueue.enQ(pWi);
}
//----< retrieve results with blocking call >--------------------------

template<typename Result>
Result ProcessWorkItem<Result>::result()
{
  return _resultsQueue.deQ();
}
//----< start child thread that dequeus work items >-------------------

template<typename Result>
void ProcessWorkItem<Result>::start()
{
  std::function<void()> threadProc =
    [&]() {
    while (true)
    {
      WorkItem<Result>* pWi = _workItemQueue.deQ();
      if (pWi == nullptr)
      {
       // std::cout << "\n  shutting down work item processing";
		_workItemQueue.enQ(nullptr);
        return;
      }
      Result result = (*pWi)();
      _resultsQueue.enQ(result);
    }
  };
  size_t cnt = 0;
  while (cnt < tCount) {
	  std::thread  *_pThread = new std::thread(threadProc);
	  _pThreads.push_back(_pThread);
	  cnt++;
  }
}
//----< clean up heap >------------------------------------------------

template<typename Result>
ProcessWorkItem<Result>::~ProcessWorkItem()
{
	for (auto thread : _pThreads)
		delete thread;
}



#endif