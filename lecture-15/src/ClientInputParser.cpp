#include "ClientInputParser.h"
#include "Client.h"
#include "ClientAction.h"
#include <cpprest/json.h>
#include <fstream>
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
		try
		{
			std::ifstream file("/resources/client_operations.json", std::ifstream::in);
			std::string s;
			web::json::value v;
			if(file)
			{
				file >> s;
				file.close();
				v = web::json::value::parse(s);
			}
			return v;
		}
		catch(web::json::json_exception& exception)
		{
			std::cout << "json parsing exception: " << exception.what();
		}
		return web::json::value{};
	}

	std::unique_ptr<Client> processBatches(web::json::array batch)
	{
		std::unique_ptr<Client> client = std::make_unique<Client>();
		for(auto it2 = batch.cbegin(); it2 != batch.cend(); ++it2)
		{
			auto operation = it2->at(U("operation")).as_string();
			auto name = it2->at(U("name")).as_string();

			std::unique_ptr<ClientAction> action = std::make_unique<ClientAction>(operation, name);

			if("create" == operation)
			{
				auto email = it2->at(U("email")).as_string();
				action->setUserEmail(email);
			}
			if("update" == operation)
			{
				auto updatedName = it2->at(U("updated_name")).as_string();
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
	std::vector<std::unique_ptr<Client>> clientList{};
//
//
//	auto client = std::make_unique<Client>();
//
//	auto action = std::make_unique<ClientAction>("create", "My User Name");
//  action->setUserEmail("myemail");
//
//	auto action1 = std::make_unique<ClientAction>("update", "My User Name");
//	action1->setUserEmail("myemail");
//	action1->setUpdatedUserName("Updated user email");
//
//  auto action2 = std::make_unique<ClientAction>("read", "My User Name");
//  action2->setUserEmail("Updated user email");
//
//  auto action3 = std::make_unique<ClientAction>("delete", "My User Name");
//  action3->setUserEmail("Updated user email");
//	client->addAction(std::move(action));
//	client->addAction(std::move(action1));
//  client->addAction(std::move(action2));
//  client->addAction(std::move(action3));
//	clientList.push_back(std::move(client));

 	try
 	{
 		web::json::value v = fileToJson();
 		auto array = v.at(U("batches")).as_array();
 		if (mNumberOfClients != array.size())
 		{
 			throw std::runtime_error{"Number of clients not matching the batch size"};
 		}

 		for(auto it = array.cbegin(); it != array.cend(); ++it)
 		{
            std::unique_ptr<Client> client = processBatches(array);
            clientList.push_back(std::move(client));
 		}
 	}
 	catch(web::json::json_exception& exception)
 	{
 		std::cout << "error parsing json: " << exception.what();
 	}

	return clientList;
}
