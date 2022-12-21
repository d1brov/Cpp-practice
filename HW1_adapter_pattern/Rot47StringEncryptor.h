#pragma once

#include <string>

#include "StringEncryptor.h"

/*
	Indexes of visible ASCII characters
*/
const int ASCII_MIN = 33,
		  ASCII_MAX = 126;

/*
	Variation of ROT-13 cipher. This implementation uses visible characters(32 - 126) from ASCII table.
*/
class Rot47StringEncryptor : public StringEncryptor {
private:
	std::string applyRot47(const std::string& str);

public:
	Rot47StringEncryptor() = default;
	std::string encrypt(const std::string& str) override;
	std::string decrypt(const std::string& str) override;
};