#pragma once
#include "Event.h"
#include "Logger.h"
#include <mutex>
#include <condition_variable>
#include <queue>


class ThreadSafeQueue
{
private:
	std::queue<Event> events;
	std::condition_variable cv;
	std::mutex m;
	bool stopFlag;


public:
	ThreadSafeQueue();
	void push(Event newevent);
	bool pop(Event &e);
	size_t getQueueSize();
	Event getLast();
	void shutDown();
	//void lockMutex();
	//std::unique_lock<std::mutex> uniqueLockMutex();
	/*void notifyOne();
	//auto getCv();

	//Event getFront();*/
};

