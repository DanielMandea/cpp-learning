#include "ClientInputParser.h"
#include "Client.h"
#include "ClientAction.h"

#include <stdexcept>
#include <string>

namespace
{
	int parseNumberOfClients(int argc, char** argv)
	{
		if (argc != 3)
		{
			throw std::runtime_error{"Expected exactly three command line arguments"};
		}

		if (std::string{argv[1]} != "clients")
		{
			throw std::runtime_error{"Unknown first argument; expected `clients`"};
		}

		return std::stoi(argv[2]);
	}
}

ClientInputParser::ClientInputParser(int argc, char** argv)
: mNumberOfClients{parseNumberOfClients(argc, argv)}
{

}

ClientInputParser::~ClientInputParser() = default;

std::vector<std::unique_ptr<Client>> ClientInputParser::buildClients() const
{
	std::vector<std::unique_ptr<Client>> clientList{};

	auto client = std::make_unique<Client>();

	auto action = std::make_unique<ClientAction>("read", "My User Name");

	client->addAction(std::move(action));

	clientList.push_back(std::move(client));

	return clientList;
}
