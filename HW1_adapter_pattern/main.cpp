#include <memory>

#include "ILogger.h"
#include "FileLogger.h"
#include "EncryptingFileLogger.h"
#include "XorStringEncryptor.h"
#include "Rot47StringEncryptor.h"

void logSomething(ILogger& fileLogger) {
	try {
		fileLogger.log("The quick brown fox jumps over the lazy dog");
	}
	catch (std::runtime_error& loggingException) {
		std::cerr << "Coudnt write to file. Reason : "
				  << loggingException.what() << '\n';
	}
}

/*
	Default file logger.
	Appends info to designated file with ISO8601 UTC timestamp.
	Uses text mode.

	"The quick brown fox jumps over the lazy dog" 
		-> "2022-12-12T12:30:45Z:The quick brown fox jumps over the lazy dog"
*/
void defaultLoggerDemo() {
	// logger initialization
	std::unique_ptr<ILogger> fileLogger;
	try {
		fileLogger = std::make_unique<FileLogger>("log_file.log");
	}
	catch (std::runtime_error& loggerInitException) {
		std::cerr << "Coudnt initialize file logger. Reason : " 
				  << loggerInitException.what() << '\n';
		return;
	}

	// logging
	logSomething(*fileLogger);
}

/*
	Xor encrypted logger adapter. 
	Uses default logger under hood and encrypts info with specified key.
	Output in hex format. (2 chars per bite)

	"The quick brown fox jumps over the lazy dog" 
		-> 2022-12-12T12:31:47Z:2D 11 1C ... 1D 16 1E 
*/
void xorEncryptedLoggerDemo() {

	std::unique_ptr<ILogger> xorFileLogger;
	try {
		xorFileLogger = 
			std::make_unique<EncryptingFileLogger>(
					"log_file.log", 
					std::make_unique<XorStringEncryptor>("key")
				);
	}
	catch (std::runtime_error& loggerInitException) {
		std::cerr << "Coudnt initialize file logger. Reason : "
				  << loggerInitException.what() << '\n';
		return;
	}

	logSomething(*xorFileLogger);
}

/*
	Implementation of ROT-47 encryption was not required.
	But I implemented it for fun.

	"The quick brown fox jumps over the lazy dog" 
		-> "2022-12-12T12:31:47Z:%96 BF:4< 3C@H? 7@I ;F>AD @G6C E96 =2KJ 5@8" 
*/
void rot47EncryptedLoggerDemo() {

	std::unique_ptr<ILogger> rot47FileLogger;
	try {
		rot47FileLogger = std::make_unique<EncryptingFileLogger>(
				"log_file.log", 
				std::make_unique<Rot47StringEncryptor>()
			);
	}
	catch (std::runtime_error& loggerInitException) {
		std::cerr << "Coudnt initialize file logger. Reason : "
			<< loggerInitException.what() << '\n';
		return;
	}

	logSomething(*rot47FileLogger);
}

int main() {
	defaultLoggerDemo();

	xorEncryptedLoggerDemo();

	rot47EncryptedLoggerDemo();
}