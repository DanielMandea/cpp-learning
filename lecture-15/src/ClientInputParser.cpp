#include "ClientInputParser.h"
#include "Client.h"
#include "ClientAction.h"

#include <cpprest/json.h>

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

	web::json::value fileToJson()
	{
        std::ifstream file{"../../resources/client_operations.json"};
        std::string s{};
        web::json::value v{};

        if (file.is_open())
        {
            while (!file.eof())
            {
                std::string aux{};
                file >> aux;
                s += aux;
            }

            file.close();
            v = web::json::value::parse(s);
        }

        return v;
	}

	std::unique_ptr<Client> processBatches(const web::json::array& batch)
	{
		std::unique_ptr<Client> client = std::make_unique<Client>();
		for (auto it2 = batch.cbegin(); it2 != batch.cend(); ++it2)
		{
			auto operation = it2->at(U("operation")).as_string();
			auto name = it2->at(U("name")).as_string();

			std::unique_ptr<ClientAction> action = std::make_unique<ClientAction>(operation, name);

			if ("create" == operation)
			{
				auto email = it2->at("email").as_string();
				action->setUserEmail(email);
			}

			if ("update" == operation)
			{
				auto updatedName = it2->at("updated_name").as_string();
				action->setUpdatedUserName(updatedName);
			}

			client->addAction(std::move(action));
		}

		return client;
	}
}

ClientInputParser::ClientInputParser(int argc, char** argv)
: mNumberOfClients{parseNumberOfClients(argc, argv)}
{

}

ClientInputParser::~ClientInputParser() = default;

std::vector<std::unique_ptr<Client>> ClientInputParser::buildClients() const
{
    web::json::value v = fileToJson();
    const auto& array = v["batches"].as_array();
    if (mNumberOfClients != array.size())
    {
        throw std::runtime_error{"Number of clients not matching the batch size"};
    }

    std::vector<std::unique_ptr<Client>> clientList{};
    for (const auto& it : array)
    {
        std::unique_ptr<Client> client = processBatches(it.as_array());
        clientList.push_back(std::move(client));
    }

	return clientList;
}
