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

	web::json::value fileToJson()
	{
		try
		{
			std::ifstream file("/resources/client_operations.json");
			std::string s;
			web::json::value v;
			if(f)
			{
				s << file.rdbuf();
				file.close();
				v = web::json::parse(s);
			}
			return v;
		}
		catch(web::json::json_exception exception)
		{
			std::cout << "json parsing exception: " << exception.what();
			break;
		}
		return web::json::value{};
	}

	Client processBatches(web::json::array batch) 
	{
		Client client{};
		for(auto it2 = batch.as_array().cbegin(); it2 != batch.as_array().cend(); ++it2)
		{
			auto operation = it2.at(U("operation")).as_string();
			auto name = it2.at(U("name")).as_string();

			ClientAction action{operation, name};

			if("create" == operation)
			{
				auto email = it2.at(U("email")).as_string();
				action.setUserEmail(email);
			}
			if("update" == operation)
			{
				auto updatedName = it2.at(U("updated_name")).as_string();
				action.setUpdatedUserName(updatedName);
			}

			client.addAction(action);
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


	auto client = std::make_unique<Client>();

	auto action = std::make_unique<ClientAction>("read", "My User Name");

	client->addAction(std::move(action));

	clientList.push_back(std::move(client));

// 	try
// 	{
// 		web::json::value v = fileToJson();
// 		auto array = v.at(U("batches")).as_array();
// 		if (mNumberOfClients != array.size())
// 		{
// 			throw std::runtime_error{"Number of clients not matching the batch size"};
// 		}

// 		for(auto it = array.cbegin(); it != array.cend(); ++it)
// 		{
// 			auto batches = it;
// 			clientList.push_back(processBatches(batch));
// 		}
// 	}
// 	catch(web::json::json_exception exception)
// 	{
// 		std::cout << "error parsing json: " << exception.what();
// 		break;
// 	}

	return clientList;
}
