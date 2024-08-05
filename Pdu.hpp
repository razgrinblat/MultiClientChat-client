#pragma once
#include <cstdint>
#include<string>
#include <vector>
#include<array>

class Pdu
{
private:
	static constexpr auto SIZE_BYTES = 4;
	static constexpr auto TWO = 2;
	std::string _name;
	uint32_t _message_size;
	std::string _message;
	std::vector<char>_buffer;

	
public:
	Pdu();
	Pdu(const std::string& name, const std::string& message);
	~Pdu() = default;

	/**
 * @brief Creates a PDU from the current object's data.
 * @return A pointer to the internal buffer containing the PDU data.
	*/
	std::vector<char>& writePdu();
	/**
 * @brief Reads a PDU from a given data buffer.
 * @param data The buffer containing the PDU data.
 * @return A reference to the current Pdu object.
 */
	void readPdu(std::vector<char>& data);

	std::string getMessage();
	
	std::string getName();

};

