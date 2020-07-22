#include "Client.h"
#include "ClientAction.h"
#include "Entity.h"

#include <cpprest/http_client.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>

namespace
{
    constexpr const char* HOSTNAME{"http://localhost:8080"};

	std::tuple<web::http::method, std::string> getEndpointByOperation(ClientAction::Operation operation)
	{
		switch (operation)
		{
			case ClientAction::Operation::CREATE:
			{
				return std::make_tuple(web::http::methods::POST, "/users");
			}

			case ClientAction::Operation::READ:
			{
				return std::make_tuple(web::http::methods::GET, "/users");
			}

			case ClientAction::Operation::UPDATE:
			{
				return std::make_tuple(web::http::methods::PUT, "/users");
			}

			case ClientAction::Operation::DELETE:
			{
				return std::make_tuple(web::http::methods::DEL, "/users");
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
	std::thread actionsThread{[this]()
	{
		while (!mActions.empty())
		{
			const auto& action = mActions.front();

			doAction(*action);

			mActions.pop();
		}
	}};

	actionsThread.detach();
}

void Client::doAction(const ClientAction& action) const
{
	auto [method, uri] = getEndpointByOperation(action.getOperation());

    web::http::http_request request{};
	request.set_method(method);
	request.set_request_uri(uri);
	web::json::value jvalue{};


	auto name = web::json::value::string(action.getUserName());
    jvalue["name"] = name;
    auto email = web::json::value::string(action.getUserEmail());
    jvalue["email"] = email;

    if (ClientAction::Operation::CREATE != action.getOperation()) {
        Entity entity{name.as_string(), email.as_string()};
        uri += "/" + std::to_string(entity.computeStorageKey());

        jvalue["email"] = web::json::value::string(action.getUpdatedUserName());
        request.set_request_uri(uri);

    }
    request.set_body(jvalue);

	web::http::client::http_client client{HOSTNAME};
	client.request(request)
	      .then([](const web::http::http_response& response)
		        {
				    std::cout << "Request executed successfully, response is: " << response.to_string() << "\n\n";
			    })
	      .wait();
}
