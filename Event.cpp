#include "Event.h"

Event::Event()
{
	eventType = EventType::Unknown;
	pid = -1;
}

Event::Event(EventType _type, std::string _processName)
{
	eventType = _type;
	Name = _processName;
	timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	pid = -1;
}

Event::Event(EventType _type, std::string _processName, int _pid){
	eventType = _type;
	pid = _pid;
	Name = _processName;
	timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

std::string Event::getType() const {
	switch (eventType) {
		case(EventType::ProcessCreated):
			return "Process Created";
		case(EventType::ProcessExited):
			return "Process Exited";
		case(EventType::FileCreated):
			return "File Created";
		case(EventType::FileDeleted):
			return "File Deleted";
		case(EventType::FileModified):
			return "File Modified";
		case(EventType::FileRenamed):
			return "File Renamed";
		default:
			return "Invalid Event Type";
	}
}

int Event::getId() const {
	if(eventType == EventType::ProcessCreated || eventType == EventType::ProcessExited)	return pid;
	return -1;
}

std::string Event::getProcessName() const {
	std::ostringstream oss;

	switch (eventType) {
		case(EventType::ProcessCreated):
			oss << "PID: " << Name;
			return oss.str();
		case(EventType::ProcessExited):
			oss << "PID: " << Name;
			return oss.str();
		case(EventType::FileCreated):
			oss << "File Path: " << Name;
			return oss.str();
		case(EventType::FileDeleted):
			oss << "File Path: " << Name;
			return oss.str();
		case(EventType::FileModified):
			oss << "File Path: " << Name;
			return oss.str();
		case(EventType::FileRenamed):
			oss << "File Path: " << Name;
			return oss.str();
		default:
			return "Invalid Event Type";
	}
	//return Name;
}

std::string Event::toString() const {
	std::ostringstream oss;

	switch (eventType) {
		case(EventType::ProcessCreated):
			oss << getType() << " | PID: " << pid << " | Process Name: " << Name << " | Time Stamp: " << getTimeStamp() << '\n';
			return oss.str();
		case(EventType::ProcessExited):
			oss << getType() << " | PID: " << pid << " | Process Name: " << Name << " | Time Stamp: " << getTimeStamp() << '\n';
			return oss.str();
		case(EventType::FileCreated):
			oss << getType()  << " | File Location: " << Name << " | Time Stamp: " << getTimeStamp() << '\n';
			return oss.str();
		case(EventType::FileDeleted):
			oss << getType()  << " | File Location: " << Name << " | Time Stamp: " << getTimeStamp() << '\n';
			return oss.str();
		case(EventType::FileModified):
			oss << getType()  << " | File Location: " << Name << " | Time Stamp: " << getTimeStamp() << '\n';
			return oss.str();
		case(EventType::FileRenamed):
			oss << getType()  << " | File Location: " << Name << " | Time Stamp: " << getTimeStamp() << '\n';
			return oss.str();
		default:
			return "Invalid Event Type";
	}
	//return Name;
}

std::string Event::getTimeStamp() const {
	//int64_t nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(
		//timestamp.time_since_epoch()
	//).count();
	std::tm local_tm{};
#ifdef _WIN32
	localtime_s(&local_tm, &timestamp); // Windows secure version
#else
	localtime_r(&timestamp, &local_tm); // POSIX thread-safe version
#endif

	// Print in YYYY-MM-DD HH:MM:SS format
	//std::cout << "Local time: "
	//	<< std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S")
	//	<< "\n";
	std::ostringstream oss;
	oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");

	return oss.str();// timestamp;
}

Event::~Event()
{
}