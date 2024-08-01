#pragma once
#include <cstdint>
#include<string>
#include <vector>
#include<array>

class Pdu
{
private:
	static constexpr int SIZE_BYTES = 4;
	static constexpr int NAME_SIZE = 30;
	static constexpr int MESSAGE_SIZE = 1020;
	uint32_t _name_size;
	std::string _name;
	uint32_t _message_size;
	std::string _message;
	std::vector<char>_buffer;
	
public:
	Pdu();
	Pdu(const std::string& name, const std::string& message);
	~Pdu();
	/**
 * @brief Creates a PDU from the current object's data.
 * @return A pointer to the internal buffer containing the PDU data.
 */
	const char* writePdu();
	/**
 * @brief Reads a PDU from a given data buffer.
 * @param data The buffer containing the PDU data.
 * @return A reference to the current Pdu object.
 */
	Pdu& readPdu(const char* data);
	
	std::string getMessage();
	
	std::string getName();

	std::string getMessageSize();
};

