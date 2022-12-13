#pragma once

#include <memory>

#include "ILogger.h"
#include "FileLogger.h"
#include "StringEncryptor.h"

/*
	Adapter class for FileLogger
	Adds encryption for logging information
*/
class EncryptingFileLogger : public ILogger {
private:
	std::unique_ptr<FileLogger> m_fileLogger;
	std::unique_ptr<StringEncryptor> m_stringEncryptor;

public:
	EncryptingFileLogger(
		const std::string& filePath, 
		std::unique_ptr<StringEncryptor> stringEncryptor
	);
	void log(const std::string& info) override;
};