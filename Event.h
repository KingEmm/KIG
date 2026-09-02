#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

enum class EventType
{
	ProcessCreated,
	ProcessExited,
	FileCreated,
	FileDeleted,
	FileModified,
	FileRenamed,
	Unknown
};

class Event
{
private:
	EventType eventType;
	std::time_t timestamp;
	int pid;
	std::string Name;
public:
	Event();
	Event(EventType _type, std::string _processName);
	Event(EventType _type, std::string _processName, int _pid);
	std::string getType() const;
	int getId() const;
	std::string getProcessName() const;
	std::string getTimeStamp() const;
	std::string toString() const;
	~Event();
};

