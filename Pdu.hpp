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
	const char* writePdu();
	Pdu& readPdu(const char* data);
	std::string getMessage();
	std::string getName();
};

