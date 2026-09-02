#include "ProcessMonitor.h"



void ProcessMonitor::monitorLoop(){
    while (running) {
       //std::unordered_map<DWORD, std::string> currentPids;
       auto currentPid = scanProcesses();
       //std::cout << "Pid: " << currentPid[2].c_str() << '\n';

       if (previousPids.size() == 0) previousPids = currentPid;

       for (const auto& pid : currentPid) {
            //std::cout<< pid.second << " | Pid: " << pid.first << '\n';
            if (previousPids.find(pid.first) == previousPids.end()) {
                queue.push(Event(EventType::ProcessCreated , pid.second, pid.first));
            }
           //std::cout << apid.first;
       }
       for (const auto& pid : previousPids) {
            //std::cout<< pid.second << " | Pid: " << pid.first << '\n';
            if (currentPid.find(pid.first) == currentPid.end()) {
                queue.push(Event(EventType::ProcessExited, pid.second, pid.first));
            }
           //std::cout << apid.first;
       }
       previousPids.swap(currentPid);

       //for (int i = 0; i < previousPids.size(); i++) {
       //    //previousPids[i].;
       //    //if(currentPid.find( previousPids[i] == 1);

       //}
       std::this_thread::sleep_for(std::chrono::seconds(1));

    }
}

std::unordered_map<DWORD, std::string> ProcessMonitor::scanProcesses(){
    std::unordered_map<DWORD, std::string> currentPid;
	HANDLE snapShot =  CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapShot != INVALID_HANDLE_VALUE) {
	    PROCESSENTRY32 pe;
	    pe.dwSize = sizeof(PROCESSENTRY32);

	    if (Process32First(snapShot, &pe)) {
            do {
                currentPid.insert({ pe.th32ProcessID, StringUtils::wcharToString(pe.szExeFile) });
            } while (Process32Next(snapShot, &pe));
	    }
        CloseHandle(snapShot);
    }

    return currentPid;
}

ProcessMonitor::ProcessMonitor(ThreadSafeQueue& _queue) : queue(_queue) {
}

void ProcessMonitor::start(){
	running = true;
    monitorThread = std::thread(&ProcessMonitor::monitorLoop, this);
}

void ProcessMonitor::stop() {
    running = false;
    monitorThread.join();
}

