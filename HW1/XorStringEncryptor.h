#pragma once

#include <string>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <iostream>

#include "StringEncryptor.h"

class XorStringEncryptor : public StringEncryptor {
private:
	std::string m_key;
	std::string applyXor(const std::string& str);

public:
	XorStringEncryptor(const std::string& key);
	std::string encrypt(const std::string& str) override;
	std::string decrypt(const std::string& str) override;
};