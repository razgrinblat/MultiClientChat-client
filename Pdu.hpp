#pragma once
#include <cstdint>
#include<string>
#include <bitset>

class Pdu
{
private:
	static constexpr int SIZE_BYTES = 5;
	static constexpr int NAME_SIZE = 30;
	static constexpr int MESSAGE_SIZE = 1024;
	uint32_t _size;
	char _name[NAME_SIZE];
	char _message[MESSAGE_SIZE];
	char* _buffer;
	
public:
	Pdu();
	Pdu(uint32_t size,const char* name,const char* message);
	~Pdu();
	/**
 * @brief Creates a PDU from the current object's data.
 *
 * Allocates memory for the PDU buffer and populates it with the size, name, and
 * message in a specific format. The buffer includes the size, name, and message
 * fields sequentially.
 *
 * @return A pointer to the internal buffer containing the PDU data.
 */
	const char* writePdu();
	/**
 * @brief Reads a PDU from a given data buffer.
 *
 * Parses the provided data buffer and extracts the size, name, and message,
 * storing them in the current Pdu object. The data buffer is expected to be
 * formatted correctly according to the PDU structure.
 *
 * @param data The buffer containing the PDU data.
 * @return A reference to the current Pdu object.
 */
	Pdu& readPdu(const char* data);
	/**
 * @brief Gets the message content of the PDU.
 *
 * Returns the message content stored in the PDU as a std::string.
 *
 * @return A string containing the message content.
 */
	std::string getMessage();
	/**
 * @brief Gets the name associated with the PDU.
 *
 * Returns the name stored in the PDU as a std::string.
 *
 * @return A string containing the name.
 */
	std::string getName();
};

