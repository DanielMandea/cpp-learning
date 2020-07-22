#include "Client.h"
#include "ClientInputParser.h"
#include "InMemoryStorageEngine.h"
#include "Server.h"

#include <iostream>

int main(int argc, char** argv)
{
	try
	{
		Server server{std::make_unique<InMemoryStorageEngine<Entity>>()};
		ClientInputParser clientInputParser{argc, argv};

		const auto clientList = clientInputParser.buildClients();
		for (const auto& client : clientList)
		{
			client->executeActions();
		}

		while (true);
	}
	catch (const web::http::http_exception& ex)
	{
		std::cout << "Received CPPRESTSDK exception: " << ex.what();
	}
	catch (const std::exception& ex)
	{
		std::cout << "Received exception: " << ex.what();
	}
	catch (...)
	{
		std::cout << "got ex\n\n\n";
	}
	
	return 0;
}
