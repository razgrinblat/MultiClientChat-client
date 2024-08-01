#include "Client.hpp"

Client::Client()
{
	initializeWinsock();
	initializeSocket();
	connectSocket();

}
Client::~Client()
{
	close(EXIT_SUCCESS);
}

void Client::initializeWinsock()
{
	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &_ws) == SOCKET_ERROR)
	{
		throw std::runtime_error("WSA Failed to Initialize\n");
	}
	std::cout << "WSA opened Succesfully\n";

}

void Client::initializeSocket()
{
	//Initialize the socket
	_client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_client_fd == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to Initialize the socket\n");
	}
	std::cout << "The socket opened Succesfully\n";
	std::cout << "============================\n";
}

void Client::connectSocket()
{
	// Set up the sockaddr_in structure
	_clientAddr.sin_family = AF_INET;
	_clientAddr.sin_port = htons(PORT);
	int result = inet_pton(AF_INET, IP, &_clientAddr.sin_addr); //convert IPv4 addresses from text to binary 
	if (result <= 0)
	{
		throw std::runtime_error("Error occurred in inet_pton.");
	}
	int connectResult = connect(_client_fd, reinterpret_cast<sockaddr*>(&_clientAddr), sizeof(_clientAddr));
	if (connectResult == SOCKET_ERROR)
	{
		throw std::runtime_error("Can't connect to server\n");
	}
}

void Client::handleExitCommand()
{
	std::string disconnected_msg = _username + " disconneted from the chat!";
	Pdu pdu("EXIT MESSAGE", disconnected_msg.c_str()); //create the pdu
	const char* data_pdu = pdu.writePdu();
	send(_client_fd, data_pdu, BUFFERSIZE, 0);
	std::cout << "by by!!";
	close(EXIT_SUCCESS);
}

void Client::handleUsersCommand()
{
	Pdu pdu("USERS", "");
	const char* data_pdu = pdu.writePdu();
	send(_client_fd, data_pdu, BUFFERSIZE, 0);
}

void Client::sendPdu(std::string user_input)
{
	uint32_t size = user_input.size(); //size of message
	Pdu pdu(_username.c_str(), user_input.c_str());
	const char* data_pdu = pdu.writePdu();
	int sendResult = send(_client_fd, data_pdu, BUFFERSIZE, 0);
	if (sendResult == SOCKET_ERROR)
	{
		std::cout << "Error in Sending";
		close(EXIT_FAILURE);
	}
}

void Client::sendMessage()
{
	std::string userInput;
	while (true)
	{
		std::getline(std::cin, userInput);
		if (userInput.empty())
		{
			continue;
		}
		if (userInput == EXIT_MESSAGE_COMMAND)
		{
			handleExitCommand();
		}
		if (userInput == USERS_COMMAND)
		{
			handleUsersCommand();
			continue;
		}
		if (userInput.size() > 0)
		{
			sendPdu(userInput);
		}
	}	
}

void Client::recieveMessage()
{
	std::array<char, BUFFERSIZE> buffer;
	while (true)
	{
		std::memset(&buffer, 0, BUFFERSIZE);
		int bytesReceived = recv(_client_fd, buffer.data(), BUFFERSIZE, 0);
		if (bytesReceived > 0)
		{
			Pdu pdu = Pdu();
			pdu.readPdu(buffer.data());
			std::cout << _color << "[" << pdu.getName() << "] " << pdu.getMessage() <<  " (" << pdu.getMessageSize() << ")" << RESET_COLOR << std::endl;
		}
	}
}

void Client::openClient()
{
	std::array<char, WELCOME_BUFFER> welcome_msg;
	int bytesReceived = recv(_client_fd, welcome_msg.data(), WELCOME_BUFFER, 0);
	welcome_msg[bytesReceived] = '\0';
	std::cout << welcome_msg.data() << std::endl; //output welcome message
	std::cout << "Enter your name:";
	do {
		std::getline(std::cin, _username);
		if (_username.size() > NAME_SIZE)
		{
			std::cout << "name is to long, enter your name again:";
		}
	} while (_username.size() > NAME_SIZE);
	addUserColor();
	
	std::thread recieving_thread = std::thread(&Client::recieveMessage, this);
	std::thread sending_thread = std::thread(&Client::sendMessage, this);
	recieving_thread.join();
	sending_thread.join();
}

void Client::addUserColor()
{
	constexpr auto RESET = "\033[0m";
	constexpr auto RED = "\033[31m";
	constexpr auto GREEN = "\033[32m";
	constexpr auto YELLOW = "\033[33m";
	constexpr auto BLUE = "\033[34m";
	constexpr auto MAGENTA = "\033[35m";
	constexpr auto CYAN = "\033[36m";
	constexpr auto WHITE = "\033[37m";

	std::unordered_map<int, std::string> clientColors = {
		{1, RED},
		{2, GREEN},
		{3, YELLOW},
		{4, BLUE},
		{5, MAGENTA},
		{6, CYAN},
		{7, WHITE}
	};

	 _color = clientColors[_client_fd % clientColors.size() + 1];
}

void Client::close(int status)
{
	//close Socket
	if (closesocket(_client_fd) == SOCKET_ERROR)
	{
		throw std::runtime_error("closing failed \n");
	}
	//WSAcleanup
	if (WSACleanup() == SOCKET_ERROR)
	{
		throw std::runtime_error("cleenup failed \n");
	}
	exit(status);
}