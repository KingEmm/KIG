#include "ThreadPool.h"


//Logger _logger();

//ThreadPool::ThreadPool(ThreadSafeQueue* _queue, Logger* _logger) : queue(_queue), logger(_logger) {
//	/*queue();
//	//logger;*/
//}

void ThreadPool::consumer(int id) {
	 Event last ;
	 //Event lastptr = &last;
	while (queue.pop(last)) {
		std::cout << "\nWorker " << id << " processed:\n";
		//if (last.getType() == "")break;
		logger.write(last);
		std::cout << last.toString();
		//last = queue.getLast();
		//std::cout << last.getType() << " | Id: " << last.getId() << " | Process Name: " << last.getProcessName() << " | TimeStamp: " << last.getTimeStamp() << '\n';
	}
}

ThreadPool::ThreadPool(ThreadSafeQueue& _queue, Logger& _logger) : queue(_queue), logger(_logger){
	numberOfWorker = 5;
}

ThreadPool::ThreadPool(ThreadSafeQueue& _queue, Logger& _logger, unsigned int _numberOfWorkers) : queue(_queue), logger(_logger){
	numberOfWorker = _numberOfWorkers;
}


void ThreadPool::start(){
	
	for (int i = 0; i < numberOfWorker; i++) {
		//workers[i] = 
		workers.emplace_back(std::thread(&ThreadPool::consumer, this, i+1));
		std::cout << "Worker" << " | Id: " << i+1 << '\n';
	}
}

void ThreadPool::stop(){
	queue.shutDown();
	for (int i = 0; i < numberOfWorker; i++) {
		//workers[i] = 
		workers[i].join();
		std::cout << "Worker" << " | Id: " << i+1 << '\n';
	}
}