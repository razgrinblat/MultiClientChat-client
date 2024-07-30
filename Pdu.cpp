#include "Pdu.hpp"

Pdu::Pdu()
{
	_size = 0;
	_buffer = nullptr;
	std::memset(_name, 0, sizeof(_name));
	std::memset(_message, 0, sizeof(_message));
}

Pdu::Pdu(uint32_t size,const char* name,const char* message)
{
	_size = size;
	_buffer = nullptr;
	strcpy_s(_name, name);
	strcpy_s(_message,message);
}

Pdu::~Pdu()
{
	delete[] _buffer;
}

const char* Pdu::writePdu()
{
	_buffer = new char[SIZE_BYTES + sizeof(_name) + sizeof(_message)];

	char size_str[5];
	int str_lenght = SIZE_BYTES;
	sprintf_s(size_str, "%04d\0",_size); //convert size to string
	std::memcpy(_buffer, size_str, str_lenght);
	std::memcpy(_buffer + str_lenght, _name, sizeof(_name));
	std::memcpy(_buffer + str_lenght + sizeof(_name), _message, sizeof(_message));
	return _buffer;
}


Pdu& Pdu::readPdu(const char* data) {
	char size_str[5] = { 0 }; // 4 digits + null terminator
	std::memcpy(size_str, data, SIZE_BYTES);
	_size = std::stoi(size_str); //convert size_str from char* to int
	std::memcpy(_name, data + SIZE_BYTES, sizeof(_name));
	std::memcpy(_message, data + SIZE_BYTES + sizeof(_name),sizeof(_message));
	
	return *this;
}

std::string Pdu::getMessage()
{
	return std::string(_message);			
}

std::string Pdu::getName()
{
	return std::string(_name);
}
