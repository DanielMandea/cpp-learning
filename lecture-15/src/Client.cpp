#include "Client.h"
#include "ClientAction.h"
#include "Entity.h"
#include "LocationNotFoundException.h"
#include "UnexpectedStatusCodeException.h"

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <iostream>
#include <stdexcept>
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
	web::http::http_request buildRequestWith(const web::http::method& method,
	                                            const std::string& uri,
	                                            const web::json::value& body = {})
    {
	    web::http::http_request request{};
	    request.set_method(method);
	    request.set_request_uri(uri);
	    if (!body.is_null())
        {
	        request.set_body(body);
        }
	    return request;
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

			try
            {
                doAction(*action);

            }
            catch (const UnexpectedStatusCodeException& e)
            {
                std::cout<<"For action "<< static_cast<int>(action->getOperation())<<"  received an error from http server "<< e.what() << "\n";
            }
            catch (const std::exception& e)
            {
                std::cout<<"For action "<< static_cast<int>(action->getOperation())<<"  std::exception message =  "<< e.what() << "\n";
            }

            mActions.pop();
		}
	}};

	actionsThread.detach();
}

void Client::doAction(const ClientAction& action) const
{
	auto [method, uri] = getEndpointByOperation(action.getOperation());
	web::json::value jvalue{};

	if(ClientAction::Operation::READ == action.getOperation()
	|| ClientAction::Operation::DELETE == action.getOperation())
    {
        web::http::http_request request = buildRequestWith(method, uri);
    }
	else
    {
        auto name = web::json::value::string(action.getUserName());
        jvalue["name"] = name;
        auto email = web::json::value::string(action.getUserEmail());
        jvalue["email"] = email;
        if (ClientAction::Operation::UPDATE == action.getOperation())
        {
            Entity entity{name.as_string(), email.as_string()};
            uri+= "/" + (std::to_string(entity.computeStorageKey()));
            jvalue["name"] = web::json::value::string(action.getUpdatedUserName());
        }
        web::http::http_request request = buildRequestWith(method, uri, jvalue);
    }
    auto name = web::json::value::string(action.getUserName());
    jvalue["name"] = name;
    auto email = web::json::value::string(action.getUserEmail());
    jvalue["email"] = email;

    if (ClientAction::Operation::CREATE != action.getOperation()) {
        Entity entity{name.as_string(), email.as_string()};
        uri += "/" + std::to_string(entity.computeStorageKey());

        jvalue["email"] = web::json::value::string(action.getUpdatedUserName());
    }
    web::http::http_request request = buildRequestWith(method, uri, jvalue);

	web::http::client::http_client client{HOSTNAME};
	client.request(request)
	      .then([&client](const web::http::http_response& response)
		        {
	                if(response.status_code() == web::http::status_codes::OK)
                    {
                        std::cout << "C: Request executed successfully, response is: " << response.to_string() << "\n\n";
                        auto json_response = response.extract_json().get();
                        auto name = json_response["user"]["name"].as_string();
                        auto email = json_response["user"]["email"].as_string();
                        std::cout << "C: result of status OK of GET request is "
                                  << " is: name = "<<name<<", email = "<<email<<"\n\n";
                    }
	                else if (response.status_code() == web::http::status_codes::Created)
                    {
	                    const auto& locationItr = response.headers().find("Location");
                        if (locationItr != response.headers().end())
                        {
                            auto location = locationItr ->second;
                            web::http::http_request request{};
                            request.set_method(web::http::methods::GET);
                            request.set_request_uri(location);
                            auto newResponse = client.request(request).get();

                            if(newResponse.status_code() == web::http::status_codes::OK)
                            {
                                auto json_response = newResponse.extract_json().get();
                                auto name = json_response["user"]["name"].as_string();
                                auto email = json_response["user"]["email"].as_string();
                                std::cout << "C: result of status OK of GET request at redirect location "
                                         <<location << " is: name = "<<name<<", email = "<<email<<"\n\n";
                            }
                            else
                            {
                                throw LocationNotFoundException("Redirect status code is not 200");
                            }
                        }
                        else
                        {
                            throw UnexpectedStatusCodeException("Location not found");
                        }
                    }
	                else
                    {
	                    try
	                    {
                            auto json_response = response.extract_json().get();
                            throw UnexpectedStatusCodeException(json_response["error_msg"].as_string());
	                    }
                        catch (const web::json::json_exception& ex)
                        {
                            throw UnexpectedStatusCodeException("received status code: "
                                                                + std::to_string(response.status_code()) +
                                                                ", but could not find json key \"error_msg\" in body");

                        }

                    }
			    })
	      .wait();
}
