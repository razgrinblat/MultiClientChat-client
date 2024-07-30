#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread> 
#include "Pdu.hpp"
#include <cstdint>
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
	static constexpr const char* IP = "127.0.0.1";
	static constexpr int PORT = 8080;
	static constexpr int BUFFERSIZE = 1058;
	static constexpr int NAME_SIZE = 30;
	
public:
	Client();
	/**
 * @brief Sends messages to the server.
 *
 * This function continuously reads user input from the console and sends it to
 * the server. Special commands `$exit$` and `$users$` are handled to disconnect
 * from the server and request the list of connected users, respectively.
 */
	void sendMessage();
	/**
 * @brief Receives messages from the server.
 *
 * This function continuously listens for messages from the server and displays
 * them. It uses a buffer to receive data and then decodes it using a PDU.
 */
	void recieveMessage();
	/**
 * @brief Starts the client and handles user input and server communication.
 *
 * This function displays the server's welcome message and prompts the user for
 * a username. It then creates separate threads for receiving and sending
 * messages.
 */
	void openClient();
	/**
 * @brief Closes the client and cleans up resources.
 *
 * This function closes the client's socket and performs necessary cleanup,
 * including shutting down Winsock.
 */
	void close();

	void addUserColor();
	


};