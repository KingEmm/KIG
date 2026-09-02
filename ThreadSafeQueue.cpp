#include "ThreadSafeQueue.h"


ThreadSafeQueue::ThreadSafeQueue() {
	stopFlag = false;
}

bool ThreadSafeQueue::pop(Event &e) {
	std::unique_lock<std::mutex> lock(m);

	cv.wait(lock, [this] {
		return !events.empty() || stopFlag;
	});

	if (stopFlag && events.empty()) {
		std::cout << "\nExiting!!!\n";
		return false;
	}
	e = std::move(events.front());
	//int id = events.front().getId();
	events.pop();
	lock.unlock();
	//*e = last;
	//cv.notify_all();
	return true;
};

void ThreadSafeQueue::push(Event newevent) {
	{
		std::lock_guard<std::mutex> lock(m);
		events.push(newevent);
	}
	cv.notify_one();
};

size_t ThreadSafeQueue::getQueueSize() {
	{
		std::lock_guard<std::mutex> lock(m);
		return events.size();
	}
}
Event ThreadSafeQueue::getLast() {
	std::unique_lock<std::mutex> lock(m);
	cv.wait(lock, [this] {
		return !events.empty() && stopFlag;
	});
	/*if (stopFlag && events.empty()) {
		std::cout << "No Data yet\n";
		return ;
	}*/
		//if (events.empty()) return Event();
		lock.unlock();
		return events.back();
}

void ThreadSafeQueue::shutDown() {

	{
		std::lock_guard<std::mutex> lock(m);
		stopFlag = true;
	}
	cv.notify_all();
}

//void ThreadSafeQueue::lockMutex() {
//}

/*td::unique_lock<std::mutex> ThreadSafeQueue::uniqueLockMutex() {
	//std::unique_lock<std::mutex> lock(m);
	//return lock;
//}*/

//auto ThreadSafeQueue::getCv() {
//	return cv;
//}

//void ThreadSafeQueue::notifyOne() {
//	cv.notify_one();
//}
//
//Event ThreadSafeQueue::getFront() {
//	return events.front();
//}