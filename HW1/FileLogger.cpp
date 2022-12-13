#include "FileLogger.h"

FileLogger::FileLogger(const std::string& filePath) {
	if (filePath.empty())
		throw std::runtime_error("File path is not specified");

	m_outputLogFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	try {
		m_outputLogFile.open(filePath, std::ios::app);
	}
	catch (std::ofstream::failure& fileOpenException) {
		throw std::runtime_error("Can't open file. Reason: " + 
									std::string(fileOpenException.what()));
	}
}

FileLogger::~FileLogger() {
	m_outputLogFile.close();
}

/*
	Some date/time customizable formater interface should be implemented 
	instead of a rigid formating function. But it's overkill for this task.
*/
std::string getCurrentFormattedTime(const char* format) {
	
	std::time_t now = std::time(NULL);
	std::tm tmNow;

	errno_t errorCode = gmtime_s(&tmNow, &now);
	if (errorCode) {
		char errorMessageBuffer[256] = {0};
		strerror_s(errorMessageBuffer, sizeof(errorMessageBuffer, errorCode));
		throw std::runtime_error(errorMessageBuffer);
	}

	std::stringstream ss;
	ss << std::put_time(&tmNow, format);

	return ss.str();
}

void FileLogger::log(const std::string& info) {
	if (info.empty())
		throw std::runtime_error("Info can't be empty");

	std::string timestr;
	try {
		//	Returns current UTC timestamp converted to ISO8601 format 
		//	(YYYY-MM-DDTHH:mm:ssZ)
		timestr = getCurrentFormattedTime("%FT%TZ");
	}
	catch (std::runtime_error& formatException) {
		throw std::runtime_error("Formatting error. Reason: " + 
									std::string(formatException.what()));
	}

	try {
		// I suppose "endl" reasonable here as it sets new line and flushes stream
		m_outputLogFile << timestr << ':' << info << std::endl; 
	}
	catch (std::ofstream::failure& fileWriteException) {
		throw std::runtime_error("Can't write to file. Reason: " + 
									std::string(fileWriteException.what()));
	}
}