#pragma once

#include <fstream>
#include <stdexcept>
#include <chrono>

#include "ILogger.h"

class FileLogger : public ILogger {
private:
	std::ofstream m_outputLogFile;

public:
	FileLogger(const std::string& filePath);
	~FileLogger();

	void log(const std::string& info) override;
};