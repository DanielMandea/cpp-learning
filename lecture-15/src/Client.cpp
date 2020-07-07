#include "Client.h"
#include "ClientAction.h"

#include <cpprest/http_client.h>

#include <iostream>
#include <stdexcept>
#include <thread>
#include <tuple>

namespace
{
	const std::string HOSTNAME = "http://localhost:8080/";

	std::tuple<web::http::method, std::string> getEndpointByOperation(ClientAction::Operation operation)
	{
		switch (operation)

		{
			case ClientAction::Operation::CREATE:
			{
				return std::make_tuple(web::http::methods::POST, "/create");
			}

			case ClientAction::Operation::READ:
			{
				return std::make_tuple(web::http::methods::GET, "/read");
			}

			case ClientAction::Operation::UPDATE:
			{
				return std::make_tuple(web::http::methods::POST, "/update");
			}

			case ClientAction::Operation::DELETE:
			{
				return std::make_tuple(web::http::methods::DEL, "/delete");
			}

			default:
			{
				throw std::runtime_error{"Unknown operation"};
			}
		}
	}
}



Client::Client()
: mActions{}
{

}

Client::~Client() = default;

void Client::addAction(std::unique_ptr<ClientAction> clientAction)
{
	mActions.push(std::move(clientAction));
}

void Client::executeActions()
{
	while (!mActions.empty())
	{
		const auto& action = mActions.front();

		doAction(*action);

		mActions.pop();
	}
}

void Client::doAction(const ClientAction& action) const
{
	std::thread doActionThread{[&action]()
	{
		auto [method, uri] = getEndpointByOperation(action.getOperation());

		web::http::uri_builder builder{uri};
		builder.append_query("user", action.getUserName());

		if (ClientAction::Operation::CREATE == action.getOperation())
		{
			builder.append_query("email", action.getUserEmail());
		}
		else if (ClientAction::Operation::UPDATE == action.getOperation())
		{
			builder.append_query("updated_name", action.getUpdatedUserName());
		}

		web::http::client::http_client client{HOSTNAME};

		client.request(method, builder.to_string())
		      .then([](const web::http::http_response& response)
			        {
					    std::cout << "Request executed successfully, server response is: " << response.to_string() << "\n\n\n";
				    })
		      .wait();
	}};

	doActionThread.detach();
}
