#include "Logger.h"

Logger::Logger() {
	file.open("EkoEDR.txt", std::ios::app);
}

void Logger::write(const Event& e) {
	if (!file.is_open()) {
		std::cout << "Error Opening File!!!";
		return;
	}
	file << e.toString();
	//file << e.getType() << " | " << e.getId() << " | " << e.getProcessName() << " | Time Stamp: " << e.getTimeStamp() << "\n";
}

Logger::~Logger() {
	if (file.is_open())
		file.close();
}