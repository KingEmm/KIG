#pragma once
#include "ThreadSafeQueue.h"

class IMonitor
{
public:
    //void initialize();

    virtual void start() = 0;

    virtual void stop() = 0;

    //void cleanup();

    virtual ~IMonitor() = default;
};