#include "Client.hpp"

Client::Client()
{
	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &_ws) == SOCKET_ERROR)
	{
		std::cout << "WSA Failed to Initialize\n";
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "WSA opened Succesfully\n";
	}
	//Initialize the socket
	_client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_client_fd == SOCKET_ERROR)
	{
		std::cout << "Failed to Initialize the socket\n";
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "The socket opened Succesfully\n";
		std::cout << "============================\n";
	}

	// Set up the sockaddr_in structure
	_clientAddr.sin_family = AF_INET;
	_clientAddr.sin_port = htons(PORT);
	inet_pton(AF_INET,IP,&_clientAddr.sin_addr); //convert IPv4 addresses from text to binary 

	int connectResult = connect(_client_fd, (sockaddr*)&_clientAddr, sizeof(_clientAddr));
	if (connectResult == SOCKET_ERROR)
	{
		std::cout << "Can't connect to server\n";
		close();
		
	}
}

void Client::sendMessage()
{
	std::string userInput;
	while(true)
	{
		std::getline(std::cin, userInput);
		if (userInput.size() == 0)
		{
			continue;
		}
		if (userInput == "$exit$")
		{
			std::string disconnected_msg = _username + " disconneted from the chat!";
			Pdu pdu(disconnected_msg.size(), "EXIT MESSAGE", disconnected_msg.c_str()); //create the pdu
			const char* data_pdu = pdu.writePdu();
			send(_client_fd, data_pdu, BUFFERSIZE, 0);
			std::cout << "by by!!";
			close();
		}
		if (userInput == "$users$")
		{
			Pdu pdu(0, "USERS", "");
			const char* data_pdu = pdu.writePdu();
			send(_client_fd, data_pdu, BUFFERSIZE, 0);
			continue;
		}
		uint32_t size = userInput.size(); //size of message
		Pdu pdu(size, _username.c_str(), userInput.c_str());
		const char* data_pdu = pdu.writePdu();
		if (userInput.size() > 0)
		{
			int sendResult = send(_client_fd, data_pdu, BUFFERSIZE, 0);
			if (sendResult == SOCKET_ERROR)
			{
				std::cout << "Error in Sending";
				close();
			}
		}
	}
}

void Client::recieveMessage()
{
	char buffer[BUFFERSIZE];
	//todo use std::array
	while (true)
	{
		std::memset(&buffer, 0, BUFFERSIZE);
		int bytesReceived = recv(_client_fd, buffer, BUFFERSIZE, 0);
		if (bytesReceived > 0)
		{
			{
				Pdu pdu = Pdu();
				pdu.readPdu(buffer);
				std::cout << "[" << pdu.getName() << "] " << pdu.getMessage() << std::endl;
			}
		}
	}
}

void Client::openClient()
{
	char welcome_msg[BUFFERSIZE];
	int bytesReceived = recv(_client_fd, welcome_msg, BUFFERSIZE, 0);
	welcome_msg[bytesReceived] = '\0';
	std::cout << welcome_msg << std::endl; //output welcome message
	std::cout << "Enter your name:";
	do {
		std::getline(std::cin, _username);
		if (_username.size() > NAME_SIZE)
		{
			std::cout << "name is to long,enter your name again:";
		}
	} while (_username.size() > NAME_SIZE);

	
	while (true)
	{
		std::thread recieving_thread = std::thread(&Client::recieveMessage, this);
		std::thread sending_thread = std::thread(&Client::sendMessage, this);
		recieving_thread.join();
		sending_thread.join();
	}
}

void Client::close()
{
	//close Socket
	if (closesocket(_client_fd) == SOCKET_ERROR)
	{
		std::cout<< "closing failed \n";
	}
	//WSAcleanup
	if (WSACleanup() == SOCKET_ERROR)
	{
		std::cout << "cleenup failed \n";
	}
	exit(EXIT_SUCCESS);
}


