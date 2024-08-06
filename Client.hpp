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
#include <atomic>
#pragma comment(lib,"ws2_32.lib")


class Client
{
private:
	WSADATA _ws;
	SOCKET _client_fd;
	struct sockaddr_in _clientAddr;
	std::string _color;
	std::string _username;
	std::thread _recieving_thread;
	std::thread _sending_thread;
	std::atomic<bool> _running_status{ true };
	static constexpr auto RESET_COLOR = "\033[0m";
	static constexpr auto IP = "127.0.0.1";
	static constexpr auto USERS_COMMAND = "$users$";
	static constexpr auto EXIT_MESSAGE_COMMAND = "$exit$";
	static constexpr auto PORT = 8080;
	static constexpr auto NAME_MAX_SIZE = 30;
	static constexpr auto DATA_SIZE = 4; 
	static constexpr auto WELCOME_BUFFER = 1138;
	static constexpr auto USERS = "USERS";
	static constexpr auto EXIT_MESSAGE = "EXIT MESSAGE";

	/**
	* @brief Initializes Winsock.
	* @throw std::runtime_error "WSA Failed to Initialize" if Winsock initialization fails.
	*/
	void initializeWinsock();
	/**
	* @brief Initializes the socket for communication.
	* @throw std::runtime_error "Failed to Initialize the socket" if socket creation fails.
	*/
	void initializeSocket();
	/**
	* @brief Connects the client socket to the server.
	*/
	void connectSocket();
	/**
 * @brief Sends a protocol data unit (PDU) to the server.
 * @param user_input The message input by the user.
 * @throw std::runtime_error "Error in Sending" if sending the PDU fails.
 */
	void sendPdu(const std::string& username, const std::string& user_input);
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
	 /*
	 * @brief Add color for every user
	 */
	void addUserColor();
	/*
	* @brief Closes the client and cleans up resources.
	*/
	void close();

	
	
public:
	Client();
	~Client();
	/**
	* @brief Starts the client and handles user input and server communication.
	*/
	void openClient();

};