#include "Client.hpp"

int main()
{
	try {
		Client client = Client();
		client.openClient();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception catch: " << e.what();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}