#include "FileSystemMonitor.h"

FileSystemMonitor::FileSystemMonitor(ThreadSafeQueue& _queue) : queue(_queue){
	directoryPath = L"C:\\Users";
	directoryHandle = INVALID_HANDLE_VALUE;
}

void FileSystemMonitor::monitorLoop() {
		//std::cout << "File Handle\n";
	if (!openDirectory()) return;
		while (running) {
			//std::cout << directoryHandle << '\n';
			processNotifications();
		}
	return;
}

bool FileSystemMonitor::openDirectory() {
		//std::cout << "File Handle\n";
	directoryHandle = CreateFileW(directoryPath.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr);
	if (directoryHandle == INVALID_HANDLE_VALUE) {
		std::cout << "Invalid File Handle\n";
		return false;
	}
	std::cout << "Good File Handle: " << directoryHandle << std::endl;
	std::cout << "Invalid File Handle: " << INVALID_HANDLE_VALUE << std::endl;
	return true;
};

void FileSystemMonitor::processNotifications() {
		//std::cout << "w vFile Handle\n";

	DWORD size;
	if (!(ReadDirectoryChangesW(directoryHandle, &buffer, 4096, true, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SIZE, &size, nullptr, nullptr))) {
		std::cout << "Can not Read Directory\n";
		return;
	}

	BYTE *ptr = buffer;
	EventType et;
	do {
		FILE_NOTIFY_INFORMATION* fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(ptr);

		//std::wstring filename(fni->FileName, fni->FileNameLength / sizeof(WCHAR));

		switch (fni->Action) {
			case FILE_ACTION_ADDED:
				et = EventType::FileCreated;
				queue.push(Event(et, StringUtils::wcharToStringWithLen(fni->FileName, fni->FileNameLength / sizeof(WCHAR))));
				//std::cout << StringUtils::wcharToStringWithLen(fni->FileName, fni->FileNameLength / sizeof(WCHAR)) << " | File Created\n";
				break;
			case FILE_ACTION_REMOVED:
				et = EventType::FileDeleted;
				queue.push(Event(et, StringUtils::wcharToStringWithLen(fni->FileName, fni->FileNameLength / sizeof(WCHAR))));
				//std::cout << StringUtils::wcharToStringWithLen(fni->FileName, fni->FileNameLength / sizeof(WCHAR))<< " | File Created\n";
				break;
			case FILE_ACTION_MODIFIED:
				et = EventType::FileModified;
				queue.push(Event(et, StringUtils::wcharToStringWithLen(fni->FileName, fni->FileNameLength / sizeof(WCHAR))));
				//std::cout << StringUtils::wcharToStringWithLen(fni->FileName, fni->FileNameLength / sizeof(WCHAR)) << " | File Created\n";
				break;
			case FILE_ACTION_RENAMED_OLD_NAME:
				et = EventType::FileRenamed;
				queue.push(Event(et, StringUtils::wcharToStringWithLen(fni->FileName, fni->FileNameLength / sizeof(WCHAR))));
				//std::cout << StringUtils::wcharToStringWithLen(fni->FileName, fni->FileNameLength / sizeof(WCHAR)) << " | File Created\n";
				break;
			case FILE_ACTION_RENAMED_NEW_NAME:
				et = EventType::FileRenamed;
				queue.push(Event(et, StringUtils::wcharToStringWithLen(fni->FileName, fni->FileNameLength / sizeof(WCHAR))));
				//std::cout << StringUtils::wcharToStringWithLen(fni->FileName, fni->FileNameLength / sizeof(WCHAR)) << " | File Created\n";
				break;
			default:
				std::cout << "Unknown Action";
				break;
		}
		//std::cout << "\n\n\n\ NEXT_ENTRY_OFFSET: " << fni->NextEntryOffset << '\n';
		if (fni->NextEntryOffset == 0) break;
		ptr += fni->NextEntryOffset;
	} while (true);
}

void FileSystemMonitor::closeDirectory() {
	CloseHandle(directoryHandle);
}

void FileSystemMonitor::start() {
	running = true;
	monitorThread = std::thread(&FileSystemMonitor::monitorLoop, this);
};

void FileSystemMonitor::stop() {
	running = false;
	monitorThread.join();
	if (directoryHandle != INVALID_HANDLE_VALUE) {
		closeDirectory();
	}
};
