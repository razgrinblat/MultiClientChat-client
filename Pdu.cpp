#include "Pdu.hpp"

Pdu::Pdu()
{
	_name.assign(NAME_SIZE, '\0');
	_message_size = 0;
	_message.assign(MESSAGE_SIZE, '\0');
}

Pdu::Pdu(const std::string& name, const std::string& message)
	: _message_size(message.size()), _name(name), _message(message) {
	_name.resize(NAME_SIZE, '\0'); 
	_message.resize(MESSAGE_SIZE, '\0'); 
}

Pdu::~Pdu()
{
}

const char* Pdu::writePdu() {
	_buffer = std::vector<char>(NAME_SIZE + SIZE_BYTES + MESSAGE_SIZE);

	
	std::array<char, SIZE_BYTES + 1> message_size_str = {};
	std::snprintf(message_size_str.data(), message_size_str.size(), "%04d", _message_size);

	std::memcpy(_buffer.data(), _name.c_str(), NAME_SIZE);
	std::memcpy(_buffer.data() + NAME_SIZE, message_size_str.data(), SIZE_BYTES);
	std::memcpy(_buffer.data() + NAME_SIZE + SIZE_BYTES, _message.c_str(), MESSAGE_SIZE);

	return _buffer.data();
}


Pdu& Pdu::readPdu(const char* pdu) {

	std::array<char, SIZE_BYTES + 1> message_size_str = {};

	std::memcpy(message_size_str.data(), pdu + NAME_SIZE, SIZE_BYTES);
	_message_size = std::stoi(message_size_str.data());
	_name.assign(pdu, NAME_SIZE);
	_message.assign(pdu + NAME_SIZE + SIZE_BYTES, MESSAGE_SIZE);
	return *this;
}

std::string Pdu::getMessage()
{
	return _message;			
}

std::string Pdu::getName()
{
	return _name;
}

std::string Pdu::getMessageSize()
{
	return std::to_string(_message_size);
}

