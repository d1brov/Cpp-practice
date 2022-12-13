#include "EncryptingFileLogger.h"

EncryptingFileLogger::EncryptingFileLogger(const std::string& filePath, std::unique_ptr<StringEncryptor> stringEncryptor) :
	m_stringEncryptor(std::move(stringEncryptor))  {
	try {
		m_fileLogger =
			std::make_unique<FileLogger>(filePath);
	}
	catch (std::runtime_error& loggerInitException) {
		throw std::runtime_error("Can't initialize FileLogger. Reason: " + 
									std::string(loggerInitException.what()));
	}
}

void EncryptingFileLogger::log(const std::string& info) {
	m_fileLogger->log(m_stringEncryptor->encrypt(info));
}