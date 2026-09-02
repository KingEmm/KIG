#pragma once
#include <windows.h>
//#include "ThreadSafeQueue.h".
#include "StringUtils.h"
#include "IMonitor.h"

class FileSystemMonitor : public IMonitor
{
private:
	ThreadSafeQueue& queue;

	std::thread monitorThread;

	std::atomic<bool> running;

	HANDLE directoryHandle;

	std::wstring directoryPath;

	//std::vector<BYTE> buffer;
	BYTE buffer[4096];

	void monitorLoop();
	bool openDirectory();
	void processNotifications();
	void closeDirectory();

public:
	void start() override;
	void stop()  override;
	FileSystemMonitor(ThreadSafeQueue& _queue);
};

