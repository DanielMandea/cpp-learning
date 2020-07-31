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
		try
		{
		    // "/resources/client_operations.json"
		    // "/Users/dragosioanfechete/Desktop/cpp-learning/lecture-15/build/bin/resources/client_operations.json"
			std::ifstream file("/Users/dragosioanfechete/Desktop/cpp-learning/lecture-15/resources/client_operations.json");
			std::string s;
			web::json::value v;
			if(file.is_open())
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
		catch (const web::json::json_exception& exception)
		{
			std::cout << "json parsing exception: " << exception.what() << std::endl;
		}
		catch (const std::exception& exception)
        {
		    std::cout << "std exception: " << exception.what();
        }
		return web::json::value{};
	}

	std::unique_ptr<Client> processBatches(const web::json::array& batch)
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

//// TESTAT, FUNCTIONEAZA
//	auto client = std::make_unique<Client>();
//
//	auto action = std::make_unique<ClientAction>("create", "My User Name");
//    action->setUserEmail("myEmail");
//    auto action1 = std::make_unique<ClientAction>("update", "My User Name");
//    action1->setUserEmail("myEmail");
//    action1->setUpdatedUserName("newEmail");
//    auto action2 = std::make_unique<ClientAction>("read", "My User Name");
//    action2->setUserEmail("newEmail");
//    auto action3 = std::make_unique<ClientAction>("delete", "My User Name");
//    action3->setUserEmail("newEmail");
//	client->addAction(std::move(action));
//	client->addAction(std::move(action1));
//	client->addAction(std::move(action2));
//	client->addAction(std::move(action3));
//
//	clientList.push_back(std::move(client));

 	try
 	{
 		web::json::value v = fileToJson();
 		const auto& array = v["batches"].as_array();
 		if (mNumberOfClients != array.size())
 		{
 			throw std::runtime_error{"Number of clients not matching the batch size"};
 		}

 		for(const auto& it : array)
 		{
            std::unique_ptr<Client> client = processBatches(it.as_array());
 			clientList.push_back(std::move(client));
 		}
 	}
 	catch(const web::json::json_exception& exception)
 	{
 		std::cout << "error parsing json: " << exception.what() << std::endl;
 	}

	return clientList;
}
