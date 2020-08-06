#include "Client.h"
#include "ClientAction.h"
#include "Entity.h"
#include "LocationNotFoundException.h"
#include "UnexpectedStatusCodeException.h"

#include <cpprest/http_client.h>
#include <cpprest/json.h>

#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace
{
    constexpr const char* HOSTNAME{"http://localhost:8080"};

    web::http::http_response performRequest(const web::http::http_request& httpRequest);

    void printHttpResponse(const web::http::http_response& httpResponse);

	std::tuple<web::http::method, std::string> getEndpointByClientAction(const ClientAction& clientAction)
	{
		switch (clientAction.getOperation())
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
				return std::make_tuple(web::http::methods::PUT,
				                       "/users/" + std::to_string(Entity{clientAction.getUserName(),
                                                                         clientAction.getUserEmail()}.
                                                                                                  computeStorageKey()));
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
	                                         const std::optional<web::json::value>& jsonBody)
	{
        web::http::http_request request{method};
        request.set_request_uri(uri);

        if (jsonBody)
        {
            request.set_body(*jsonBody);
        }

        return request;
    }

    void handleStatusOkHttpResponse(const web::http::http_request& httpRequest,
                                    const web::http::http_response& httpResponse)
    {
        if (web::http::methods::POST == httpRequest.method())
        {
            throw UnexpectedStatusCodeException{"Unexpected status code `200-OK` from a POST request"};
        }

        if (web::http::methods::POST == httpRequest.method())
        {
            printHttpResponse(httpResponse);
        }
    }

    void handleStatusCreatedHttpResponse(const web::http::http_request& httpRequest,
                                         const web::http::http_response& httpResponse)
    {
        if (web::http::methods::POST != httpRequest.method())
        {
            throw UnexpectedStatusCodeException{"Unexpected status code `201-Created` from a non-POST request"};
        }

        const auto& responseHeaders = httpResponse.headers();
        const auto locationFindItr = responseHeaders.find("Location");
        if (responseHeaders.end() == locationFindItr)
        {
            throw LocationNotFoundException{"Missing `Location` header in `201-Created` HTTP response"};
        }

        performRequest(buildRequestWith(web::http::methods::GET, locationFindItr->second, std::nullopt));
    }

    void handleUnexpectedStatusHttpResponse(const web::http::http_response& httpResponse)
    {
        const auto exceptionMessage = "Unexpected status code `" + std::to_string(httpResponse.status_code()) + "`";

        try
        {
            throw UnexpectedStatusCodeException{exceptionMessage + ": " +
                                                httpResponse.extract_json().get().at("error_msg").as_string()};
        }
        catch (const web::json::json_exception& ex)
        {
            throw UnexpectedStatusCodeException{exceptionMessage + "`, but failed to parse error_msg: " + ex.what()};
        }
    }

    void printHttpResponse(const web::http::http_response& httpResponse)
    {
        std::cout << "C: result with status OK of GET request is: ";

        const auto jsonReply = httpResponse.extract_json().get();
        const auto& userJsonObjectList = jsonReply.as_array();

        for (const auto& itr : userJsonObjectList)
        {
            const auto& userJsonObject = itr.at("user");
            std::cout << "name = `" << userJsonObject.at("name").as_string() << "`, "
                      << "email = `" << userJsonObject.at("email").as_string() << "`, "
                      << "key = `" << userJsonObject.at("key").as_string() << "`; ";
        }
        std::cout << "\n";
    }

    using HttpResponseHandler = std::function<void(const web::http::http_request&, const web::http::http_response&)>;
    using HttpResponseHandlerMap = std::unordered_map<web::http::status_code, HttpResponseHandler>;

    const HttpResponseHandlerMap& getHttpResponseHandlerMap()
    {
        static HttpResponseHandlerMap httpResponseHandlerMap{
                                                { web::http::status_codes::OK, handleStatusOkHttpResponse },
                                                { web::http::status_codes::Created, handleStatusCreatedHttpResponse } };

        return httpResponseHandlerMap;
    }

    web::http::http_response performRequest(const web::http::http_request& httpRequest)
    {
        return web::http::client::http_client{HOSTNAME}.request(httpRequest).get();
    }

    void handleHttpResponse(const web::http::http_request& httpRequest,
                            const web::http::http_response& httpResponse,
                            bool handleAutomatically)
    {
        const auto& responseHandlerMap = getHttpResponseHandlerMap();

        const auto handlerFindItr = responseHandlerMap.find(httpResponse.status_code());
        if (responseHandlerMap.cend() != handlerFindItr)
        {
            if (handleAutomatically)
            {
                handlerFindItr->second(std::cref(httpRequest), std::cref(httpResponse));
            }
        }
        else
        {
            handleUnexpectedStatusHttpResponse(httpResponse);
        }
    }

    std::string findKeyInUserList(const web::json::array& userList, const std::string& userName)
    {
        const auto userNameFindItr = std::find_if(userList.begin(),
                     userList.end(),
                     [&userName](const auto& jsonObject)
                     {
                         return userName == jsonObject.at("user").as_object().at("name").as_string();
                     });

        if (userNameFindItr == userList.end())
        {
            throw std::runtime_error{"could not match username `" + userName + "` against user list"};
        }

        return userNameFindItr->at("user").as_object().at("key").as_string();
    }

    void executeAction(const ClientAction& action)
    {
        std::optional<web::json::value> jsonBody{};
        const auto [method, uri] = getEndpointByClientAction(action);
        std::string newUri{uri};
        if (ClientAction::Operation::CREATE == action.getOperation())
        {
            jsonBody.emplace(
                    web::json::value::object(
                            std::vector<std::pair<std::string, web::json::value>>
                                    {
                                            {"name", web::json::value::string(action.getUserName())},
                                            {"email", web::json::value::string(action.getUserEmail())}
                                    }));
        }
        else {
            auto request = buildRequestWith(web::http::methods::GET, "/users", std::nullopt);
            const auto response = performRequest(request);
            handleHttpResponse(request, response, false);
            const auto key = findKeyInUserList(response.extract_json().get().as_array(), action.getUserName());
            newUri = "/users/" + key;
            if (ClientAction::Operation::UPDATE == action.getOperation()) {
                jsonBody.emplace(
                        web::json::value::object(
                                std::vector<std::pair<std::string, web::json::value>>
                                        {
                                                {"name", web::json::value::string(action.getUserName())},
                                                {"updated_name", web::json::value::string(action.getUpdatedUserName())}
                                        }));
            }
        }
            handleHttpResponse(buildRequestWith(method, newUri, jsonBody),
                               performRequest(buildRequestWith(method, newUri, jsonBody)),
                               true);
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
                executeAction(*action);
            }
			catch (const UnexpectedStatusCodeException& e)
            {
                std::cout << "C: For action " << static_cast<int>(action->getOperation())
                          << ", received unexpected status code: " << e.what() << "\n";
            }
			catch (const std::exception& e)
            {
			    std::cout << "C: For action " << static_cast<int>(action->getOperation())
			              << ", received std::exception " << e.what() << "\n";
            }

            mActions.pop();
		}
	}};

	actionsThread.detach();
}
