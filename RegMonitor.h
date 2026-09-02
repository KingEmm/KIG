#pragma once
#include <windows.h>
#include "IMonitor.h"
#include <thread>
#include <atomic>
#include <string>
#include <unordered_map>


struct RegistryValue {
	std::wstring name;
	DWORD type;
	//DWORD data;
	std::wstring data;
};

class RegMonitor : public IMonitor
{
	std::thread monitorThread;

	std::atomic<bool> running;

	HKEY rootKey;
	HKEY registryKey;
	std::wstring path;

	HANDLE stopEvent;
	HANDLE regEvent;

	ThreadSafeQueue& queue;

	bool openRegistryKey();
	void closeRegistryKey() const;

	void monitorLoop();
	bool watchRegistry();

	std::unordered_map<std::wstring, RegistryValue> snapshotRegistry();
	void readRegistryValue() const;

	std::unordered_map<std::wstring, RegistryValue> compareSnapshots(std::unordered_map<std::wstring, RegistryValue> oldSnaps);

	void queryRegistryChanges();
	void closeEventHandles() const;
public:
	RegMonitor(ThreadSafeQueue &_queue, HKEY _rootKey, std::wstring _path);
	RegMonitor(ThreadSafeQueue &_queue);
	~RegMonitor();
	void start() override;

	void stop() override;
};

