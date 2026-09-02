#include "RegMonitor.h"


void RegMonitor::start() {
	running = true;
	regEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
	if (regEvent == NULL) {
		wprintf(L"CreateEvent failed: %ld\n", GetLastError());
		return;
	}

	stopEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
	if (stopEvent == NULL) {
		wprintf(L"CreateEvent failed: %ld\n", GetLastError());
		CloseHandle(regEvent);
		return;
	}

	if (!openRegistryKey()) {
		std::cout << "Failed TO Open Registry\n";
		closeEventHandles();
		return;
	}

	monitorThread = std::thread(&RegMonitor::monitorLoop, this);
}

void RegMonitor::monitorLoop() {
	if (regEvent == NULL) {
		wprintf(L"RegEvent failed: %ld\n", GetLastError());
	}

	HANDLE results[2] = { stopEvent, regEvent };
	auto oldReg = snapshotRegistry();
	while (running) {
		if (!watchRegistry()) {
			break;
		}
		std::wcout << L"\n--- Current Values ---\n";

		oldReg = compareSnapshots(oldReg);
	//queryRegistryChanges();
	}

	std::cout << "Registry monitor thread exiting.\n";
}

bool RegMonitor::openRegistryKey() {
	LSTATUS	lStatus = RegOpenKeyExW(rootKey, path.c_str(), 0, KEY_READ | KEY_NOTIFY, &registryKey);
	if (lStatus == ERROR_SUCCESS) {
			std::cout << lStatus << "\n!!! Registry Opened Successfully!!!\n";
		return true;
	}
	
	else if (lStatus == ERROR_INVALID_PARAMETER) {
		std::cout << "!!! File Location Not found !!!\n";
		return false;
	}
	
	else if (lStatus == ERROR_ACCESS_DENIED) {
		std::cout << "!!! ACCESS DENIED !!!\n!!! Please Run as Administrator !!!";
	}
	return false;
}

bool RegMonitor::watchRegistry() {
	LSTATUS lStatus = RegNotifyChangeKeyValue(registryKey, FALSE, REG_NOTIFY_CHANGE_LAST_SET, regEvent, TRUE);
	if (lStatus != ERROR_SUCCESS) {
		std::cout << lStatus << "\n!!! Opened for monitoring !!!\n";
		return false;
	}
	
	//else if (lStatus == ERROR_INVALID_PARAMETER) {
	//	std::cout << "!!! File Location Not found !!!\n";
	//}
	//
	//else if (lStatus == ERROR_ACCESS_DENIED) {
	//	std::cout << "!!! ACCESS DENIED !!!\n!!! Please Run as Administrator !!!";
	//}
	
	HANDLE events[] = {stopEvent, regEvent};
	auto result = WaitForMultipleObjects(2, events, FALSE, INFINITE);
		//readRegistryValue();

	if (result == WAIT_OBJECT_0 + 1) {
		//std::cout << "\n !!! New Event Created !!!\n";
		snapshotRegistry();
		ResetEvent(regEvent);
		return true;
	}
	else if (result == WAIT_OBJECT_0 + 0) {
		std::cout << "\n!!! Shutdown Signaled !!!\n";
		return false;
	}
	else if (result == WAIT_FAILED) {
		std::cout << "Failed To Wait" << GetLastError();
	}
	return false;
}

std::unordered_map<std::wstring, RegistryValue> RegMonitor::snapshotRegistry() {
	std::unordered_map<std::wstring, RegistryValue> snaps;

	wchar_t buffer[256];
	DWORD bufferSize;
	DWORD index = 0;
	DWORD type;
	BYTE data[1024];
	DWORD dataSize;


	while(true) {
		bufferSize = 256;
		dataSize = 1024;
		auto result = RegEnumValueW(registryKey, index, buffer, &bufferSize, NULL, &type, data, &dataSize);

		if (result == ERROR_NO_MORE_ITEMS) {
			break;
		}
		if (result != ERROR_SUCCESS) {
			break;
		}
		RegistryValue regVal;
		regVal.name = std::wstring((wchar_t*)&buffer, bufferSize);
		regVal.data = std::wstring((wchar_t*) & data, dataSize / sizeof(wchar_t));
		regVal.type = type;

		snaps.emplace(std::wstring((wchar_t*)&buffer, bufferSize), regVal);
//std::wcout <</* "Index: " << index << */" | Name: " << std::wstring((wchar_t*)&buffer, bufferSize) << " | Data: ";
//				if (type == REG_SZ) std::wcout << std::wstring((wchar_t*)&data, dataSize / sizeof(wchar_t));
//				else if (type == REG_DWORD) std::wcout << *(DWORD*)&data;
//				else if (type == REG_BINARY) std::wcout << L"Binary data";
//				else  std::wcout << L"<Type: " << type << ">";
//				std::cout << "\n";

		index++;
	}
	return snaps;
}

std::unordered_map<std::wstring, RegistryValue> RegMonitor::compareSnapshots(std::unordered_map<std::wstring, RegistryValue> oldSnaps) {
	auto newSnaps = snapshotRegistry();
	for (auto& snap : oldSnaps) {
		if (!(newSnaps.find(snap.first) != newSnaps.end())) {
			//if(newSnaps[snap.first].data != oldSnaps[snap.first].data){
			auto type = oldSnaps.find(snap.first)->second.type;
			std::wcout << "Removed | Name: " << oldSnaps.find(snap.first)->second.name << " | Data: ";
			if (type == REG_SZ) std::wcout << oldSnaps.find(snap.first)->second.data;
			else if (type == REG_DWORD) std::wcout << oldSnaps.find(snap.first)->second.data;
			else if (type == REG_BINARY) std::wcout << L"Binary data";
			else  std::wcout << L"<Type: " << type << ">";
			std::cout << "\n";

			//}
		}
	}
	
	for (auto& snap : newSnaps) {
		if (!(oldSnaps.find(snap.first) != oldSnaps.end())) {
			//if(oldSnaps[snap.first].data != newSnaps[snap.first].data){
			auto type = snap.second.type;
			std::wcout << "Added | Name: " << snap.first << " | Data: ";
			if (type == REG_SZ) std::wcout << snap.second.data;
			else if (type == REG_DWORD) std::wcout << snap.second.data;
			else if (type == REG_BINARY) std::wcout << L"Binary data";
			else  std::wcout << L"<Type: " << type << ">";
			std::cout << "\n";
		}
		else {
			if (newSnaps.at(snap.first).data != oldSnaps.at(snap.first).data) {
				std::wcout << "Old: " << oldSnaps.find(snap.first)->second.name << " | " << oldSnaps.find(snap.first)->second.data;
				std::wcout << " | New: " << newSnaps.find(snap.first)->second.name << " | " << oldSnaps.find(snap.first)->second.data;
				auto type = snap.second.type;
				std::wcout << "\nValue Changed | Name: " << oldSnaps.find(snap.first)->second.name << " | Data: ";
				if (type == REG_SZ) std::wcout << newSnaps.find(snap.first)->second.data;
				else if (type == REG_DWORD) std::wcout << newSnaps.find(snap.first)->second.data;
				else if (type == REG_BINARY) std::wcout << L"Binary data";
				else  std::wcout << L"<Type: " << type << ">";
				std::cout << "\n";

			}
		}
	}

	return newSnaps;
}
	/*	else {
			if (newSnaps.at(snap.first).name != oldSnaps.at(snap.first).name) {
				std::wcout << "Old: " << oldSnaps.find(snap.first)->second.name << " | " << oldSnaps.find(snap.first)->second.data;
				std::wcout << " | New: " << newSnaps.find(snap.first)->second.name << " | " << oldSnaps.find(snap.first)->second.data;
				auto type = snap.second.type;
				std::wcout << "\nName Changed | Name: " << oldSnaps.find(snap.first)->second.name << " | Data: ";
				if (type == REG_SZ) std::wcout << oldSnaps.find(snap.first)->second.data;
				else if (type == REG_DWORD) std::wcout << oldSnaps.find(snap.first)->second.data;
				else if (type == REG_BINARY) std::wcout << L"Binary data";
				else  std::wcout << L"<Type: " << type << ">";
				std::cout << "\n";
			}
		}*/
	//if (oldSnaps.size() == newSnaps.size()) {
	//	for (auto& snap : oldSnaps) {
	//		if (!(newSnaps.find(snap.first) != newSnaps.end())) {
	//			if (newSnaps.at(snap.first).data != oldSnaps.at(snap.first).data || oldSnaps.at(snap.first).name != newSnaps.at(snap.first).name) {
	//				std::wcout << "Old: " << oldSnaps[snap.first].name << " | " << oldSnaps[snap.first].data;
	//				std::wcout << " | New: " << newSnaps[snap.first].name << " | " << newSnaps[snap.first].data;
	//				auto type = snap.second.type;
	//				std::wcout << "\nChanged | Name: " << oldSnaps[snap.first].name << " | Data: ";
	//				if (type == REG_SZ) std::wcout << newSnaps[snap.first].data;
	//				else if (type == REG_DWORD) std::wcout << newSnaps[snap.first].data;
	//				else if (type == REG_BINARY) std::wcout << L"Binary data";
	//				else  std::wcout << L"<Type: " << type << ">";
	//				std::cout << "\n";
	//			}
	//			//}
	//		}
	//		//}
	//	}
	//}

void RegMonitor::readRegistryValue() const{}

void RegMonitor::queryRegistryChanges() {}

void RegMonitor::closeRegistryKey() const { RegCloseKey(registryKey); }

void RegMonitor::closeEventHandles() const {
		CloseHandle(regEvent);
		CloseHandle(stopEvent);
}

void RegMonitor::stop() {
	if (!running && !monitorThread.joinable())
	{
		return;
	}

	running = false;
	SetEvent(stopEvent);
	  
	monitorThread.join();

	closeRegistryKey();
	CloseHandle(regEvent);
	CloseHandle(stopEvent);
}

RegMonitor::RegMonitor(ThreadSafeQueue& _queue) : queue(_queue){}

RegMonitor::RegMonitor(ThreadSafeQueue& _queue, HKEY _rootKey, std::wstring _path) : queue(_queue) {
	rootKey = _rootKey;
	path = _path;
};

RegMonitor::~RegMonitor()
{
	if (monitorThread.joinable()) stop();
}

		/*auto result = WaitForSingleObject(stopEvent, 0);
		if (result == WAIT_OBJECT_0) {
			std::cout << "\nShutdown Signaled\n";
			break;
		}*/

		//return false;
		//ResetEvent(stopEvent);
		//watchRegistry();
		//DWORD waitResult = WaitForSingleObject(stopEvent, 0);
		//if (waitResult == WAIT_OBJECT_0) {
		//	// stopEvent was signaled -> time to exit
		//	wprintf(L"\n\nWorker: Stop signal received. Exiting...\n");
		//	break;
		//}