#include "Client.h"
#include "ClientInputParser.h"
#include "Server.h"

#include <iostream>

int main(int argc, char** argv)
{
	try
	{
		Server server{};
		ClientInputParser clientInputParser{argc, argv};

		const auto clientList = clientInputParser.buildClients();
		for (const auto& client : clientList)
		{
			client->executeActions();
		}
	}
	catch (const std::exception& ex)
	{
		std::cout << "Received exception: " << ex.what();
	}
	
	return 0;
}
