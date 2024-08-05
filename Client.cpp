#include "Client.hpp"

Client::Client()
{
	initializeWinsock();
	initializeSocket();
	connectSocket();
}
Client::~Client()
{
	close();
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


void Client::sendPdu(const std::string& username, const std::string& user_input)
{
	std::array<char, 5> pdu_size_str = {};
	pdu_size_str.fill(0);
	Pdu pdu(username.c_str(), user_input.c_str());
	std::vector<char> data_pdu = pdu.writePdu();
	std::snprintf(pdu_size_str.data(), pdu_size_str.size(), "%04d", data_pdu.size());
	send(_client_fd, pdu_size_str.data(), pdu_size_str.size(), 0); //send the pdu size
	int sendResult = send(_client_fd, data_pdu.data(), data_pdu.size(), 0);
	if (sendResult == SOCKET_ERROR)
	{
		throw std::runtime_error("Error in Sending");
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
			sendPdu(EXIT_MESSAGE, _username + " disconnected from the chat!");
			std::cout << "bye bye!!";
			close();
			break;
		}
		if (userInput == USERS_COMMAND)
		{
			sendPdu(USERS, "");
			continue;
		}
		if (userInput.size() > 0)
		{
			sendPdu(_username,userInput);
		}
	}	
}

void Client::recieveMessage()
{
	std::array<char, DATA_SIZE+1> pdu_length_str = {};
	while (true)
	{
		pdu_length_str.fill(0);
		int bytesReceived = recv(_client_fd, pdu_length_str.data(), DATA_SIZE, 0);
		if (bytesReceived == DATA_SIZE)
		{
			int pdu_length = std::atoi(pdu_length_str.data());
			std::vector<char> buffer(pdu_length);
			recv(_client_fd, buffer.data(), pdu_length, 0);
			Pdu pdu = Pdu();
			pdu.readPdu(buffer);
			std::cout << _color << "[" << pdu.getName() << "] " << pdu.getMessage() << RESET_COLOR << std::endl;
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
		if (_username.size() > NAME_MAX_SIZE)
		{
			std::cout << "name is to long, enter your name again:";
		}
	} while (_username.size() > NAME_MAX_SIZE);
	addUserColor();
	
	_recieving_thread = std::thread(&Client::recieveMessage, this);
	_sending_thread = std::thread(&Client::sendMessage, this);
	_recieving_thread.join();
	_sending_thread.join();
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

void Client::close()
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
}