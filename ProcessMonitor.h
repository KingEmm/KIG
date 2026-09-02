#pragma once

#include "IMonitor.h"
#include <unordered_map>
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include "StringUtils.h"



class ProcessMonitor : public IMonitor
{
private:

    ThreadSafeQueue& queue;

    std::thread monitorThread;

    std::atomic<bool> running;

    std::unordered_map<DWORD, std::string> previousPids;

    void monitorLoop();

    std::unordered_map<DWORD, std::string> scanProcesses();

    //std::string wcharToString(const WCHAR* wstr);

public:

    ProcessMonitor(ThreadSafeQueue& _queue);

    void start() override;

    void stop() override;
};
