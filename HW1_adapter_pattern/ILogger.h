#pragma once // todo set IFNDEF

#include <string>

class ILogger {
	public:
		virtual ~ILogger() = default;

		/*
			log(...) is non const because "ostream <<" operation is non const. 
			So file writing with this implementation would be impossible.
		*/
		virtual void log(const std::string& info) = 0;
};