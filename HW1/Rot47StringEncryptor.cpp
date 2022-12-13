#include "Rot47StringEncryptor.h"

#include <iostream>

std::string Rot47StringEncryptor::applyRot47(const std::string& str) {
	const std::string::size_type strLen = str.length();

	std::string processedStr;
	processedStr.resize(strLen);

	for (std::string::size_type i = 0; i < strLen; i++) {
		if (str[i] >= '!' && str[i] <= '~') {
			// boring expression
			// processedStr[i] = 33 + (str[i] - 14) % 94

			// fun expression
			processedStr[i] = '!' + (str[i] - '!' + ('~' - '!') / 2 + 1) % ('~' - '!' + 1);
		}
		else
			processedStr[i] = str[i];
	}

	return processedStr;
}

std::string Rot47StringEncryptor::encrypt(const std::string& str) {
	return applyRot47(str);
}

std::string Rot47StringEncryptor::decrypt(const std::string& str) {
	return applyRot47(str);
}