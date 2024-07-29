#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread> 
#include "Pdu.hpp"
#include <cstdint>
#pragma comment(lib,"ws2_32.lib")


class Client
{
private:
	WSADATA _ws;
	SOCKET _client_fd;
	struct sockaddr_in _clientAddr;
	std::string _username;
	static constexpr const char* IP = "127.0.0.1";
	static constexpr int PORT = 8080;
	static constexpr int BUFFERSIZE = 1058;
	
public:
	Client();
	void sendMessage();
	void recieveMessage();
	void openClient();
	void close();
	


};