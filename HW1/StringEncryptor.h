#pragma once

#include <string>
#include <stdexcept>

class StringEncryptor {
public:
	virtual ~StringEncryptor() = default;
	virtual std::string encrypt(const std::string& str) = 0;
	virtual std::string decrypt(const std::string& str) = 0;
};