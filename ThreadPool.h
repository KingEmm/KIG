#pragma once
#include <vector>
#include <thread>
#include "ThreadSafeQueue.h"

class ThreadPool
{
private:

    std::vector<std::thread> workers;

    ThreadSafeQueue& queue;

    Logger& logger;

    size_t numberOfWorker;


public:
    void consumer(int id);

    ThreadPool(ThreadSafeQueue& _queue, Logger& _logger);

    ThreadPool(ThreadSafeQueue& _queue, Logger& _logger, unsigned int _numberOfWorkers);

    void start();

    void stop();
};

