#include "Pdu.hpp"

Pdu::Pdu()
{
	_name = "";
	_message_size = 0;
	_message = "";
}

Pdu::Pdu(const std::string& name,const std::string& message)
{
	_name = std::move(name);
	_message = std::move(message);
	_message_size = name.size() + message.size() + TWO;
}


std::vector<char>& Pdu::writePdu() {
	_buffer = std::vector<char>();

	std::array<char, SIZE_BYTES + 1> message_size_str = {};
	std::snprintf(message_size_str.data(), message_size_str.size(), "%04d", _message_size);

	_buffer.insert(_buffer.end(), message_size_str.begin(), message_size_str.end() - 1);
	_buffer.insert(_buffer.end(), _name.begin(), _name.end());
	_buffer.push_back('\0');
	_buffer.insert(_buffer.end(), _message.begin(), _message.end());
	_buffer.push_back('\0');

	return _buffer;
}


void Pdu::readPdu(std::vector<char>& data) {

	auto nameEnd = std::find(data.begin(), data.end(), '\0');
	_name.assign(data.begin(), nameEnd);
	auto messageStart = nameEnd + 1;
	auto messageEnd = std::find(messageStart, data.end(), '\0');
	_message.assign(messageStart, messageEnd);
}

std::string Pdu::getMessage()
{
	return _message;			
}

std::string Pdu::getName()
{
	return _name;
}
