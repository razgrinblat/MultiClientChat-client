#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread> 
#include "Pdu.hpp"
#include <cstdint>
#include <array>
#include <unordered_map>
#pragma comment(lib,"ws2_32.lib")


class Client
{
private:
	WSADATA _ws;
	SOCKET _client_fd;
	struct sockaddr_in _clientAddr;
	std::string _color;
	std::string _username;
	const std::string RESET_COLOR = "\033[0m";
	static constexpr auto IP = "127.0.0.1";
	static constexpr auto USERS_COMMAND = "$users$";
	static constexpr auto EXIT_MESSAGE_COMMAND = "$exit$";
	static constexpr auto PORT = 8080;
	static constexpr auto BUFFERSIZE = 1054;
	static constexpr auto NAME_SIZE = 30;
	static constexpr auto WELCOME_BUFFER = 2000;
	
public:
	Client();
	~Client();
	void initializeWinsock();
	void initializeSocket();
	void connectSocket();

	void handleExitCommand();
	void handleUsersCommand();
	void sendPdu(std::string user_input);
	/**
 * @brief Sends user input to the server.
 * Handles `$exit$` to disconnect and `$users$` to request a user list.
 */
	void sendMessage();
	/**
 * @brief Receives messages from the server.
 * It uses a buffer to receive data and then decodes it using a PDU.
 */
	void recieveMessage();
	/**
 * @brief Starts the client and handles user input and server communication.

 */
	void openClient();
	/// @brief Add color for every user
	void addUserColor();
	/*
 * @brief Closes the client and cleans up resources.
 */
	void close(int status);
	


};