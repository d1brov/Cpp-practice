#include "XorStringEncryptor.h"

//XorStringEncryptor::XorStringEncryptor(const std::string& key) 
//	try : StringEncryptor(key) {
//}
//catch (std::runtime_error stringEncryptorInitException) {
//	throw std::runtime_error("Cant init StringEncryptor. Reason: " + 
//								std::string(stringEncryptorInitException.what()));
//}

XorStringEncryptor::XorStringEncryptor(const std::string& key) {
	if (key.empty())
		throw std::runtime_error("Key can't be empty");
	m_key = key;
}

std::string XorStringEncryptor::applyXor(const std::string& str) {
	
	if (str.empty())
		throw std::runtime_error("Input string can't be empty");

	const std::string::size_type strLen = str.length();
	const std::string::size_type keyLen = m_key.length();

	if (keyLen < strLen)
		std::cerr << 
		"WARNING key length less than string length. Encryption can be compromised.\n";

	std::string processedStr;
	processedStr.resize(strLen);

	// xor-ing string with key
	for (std::string::size_type t = 0; t < strLen; t++) {
		for (std::string::size_type k = 0; k < keyLen; k++) {
			processedStr[t] = str[t] ^ m_key[k];
		}
	}

	return processedStr;
}

/*
	Converts string to string of hex values.
	"hello" -> "68 65 6C 6C 6F"
*/
std::string stringToHexString(const std::string& str) {
	std::stringstream hexStream;
	for (auto ch : str)
		hexStream << std::hex << std::setfill('0')
		<< std::setw(2)
		<< std::uppercase << (int)ch << ' ';
	return hexStream.str();
}

std::string XorStringEncryptor::encrypt(const std::string& str) {
	return stringToHexString(applyXor(str));
}

std::string XorStringEncryptor::decrypt(const std::string& str) {
	// YAGNI :)
	throw std::runtime_error("XOR decryptiopn not implemented");
}