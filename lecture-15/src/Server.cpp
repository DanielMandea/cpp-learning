#include "Server.h"
#include "StorageEngineIntf.h"

#include <iostream>
#include <functional>
#include <unordered_map>

namespace
{
    constexpr const char* HOSTNAME{"http://localhost:8080"};

    using MethodToHandlerMap = std::unordered_map<web::http::method,
                                                  std::function<void(const web::http::http_request&)>>;
}

Server::Server(std::unique_ptr<StorageEngineIntf<Entity>> storageEngine)
: mListener{HOSTNAME},
  mStorageEngine{std::move(storageEngine)}
{
    for (const auto& [requestMethod, requestHandler] : MethodToHandlerMap
                                                       {
                                                           {
                                                               web::http::methods::POST,
                                                               std::bind(&Server::handlePostRequest,
                                                                         this,
                                                                         std::placeholders::_1)
                                                           },
                                                           {
                                                               web::http::methods::GET,
                                                               std::bind(&Server::handleGetRequest,
                                                                         this,
                                                                         std::placeholders::_1)
                                                           },
                                                           {
                                                               web::http::methods::PUT,
                                                               std::bind(&Server::handlePutRequest,
                                                                         this,
                                                                         std::placeholders::_1)
                                                           },
                                                           {
                                                               web::http::methods::DEL,
                                                               std::bind(&Server::handleDeleteRequest,
                                                                         this,
                                                                         std::placeholders::_1)
                                                           }
                                                       })
    {
        mListener.support(requestMethod, requestHandler);
    }

    mListener.open().wait();
}

void Server::handlePostRequest(const web::http::http_request& request)
{
    // POST /users?name=MyUserName&email=useremailatgmail.com
    //
    // create a new user with the data retrieved from the HTTP URI query,
    // then reply with 201 (Created) and redirect to /users/123 (123 = ID assigned to the freshly created user).
    // if there is missing data in the URI query, reply with BadRequest
    // if any internal error came up on the server, reply with InternalServerError

    try {
        auto pathList = web::http::uri::split_path(request.request_uri().path());
        web::json::value json_value;
        std::string name{};
        std::string email{};

        std::cout << "S: Received request at `" << request.request_uri().path() << "`, method = POST\n";

        if ((pathList.size() == 1 && pathList[0] != "users") || pathList.size() > 1) {
            web::json::value json_error{};
            json_error["error_msg"] = web::json::value::parse("unexpected path structure");

            std::cout << "S: Sending reply to earlier request with status code `BadRequest`, body="
                      << json_error.serialize() << std::endl;

            request.reply(web::http::status_codes::BadRequest, json_error);
        }
        else {
            auto task = request.extract_json();
            task.wait();
            json_value = task.get();
            name = json_value.as_object().at("name").as_string();
            email = json_value.as_object().at("email").as_string();

            Entity entity{name, email};
            mStorageEngine->create(entity);
            std::string newLocation = "/users/" + std::to_string(entity.computeStorageKey());
            web::http::http_response response{};
            auto& headers = response.headers();
            headers["Location"] = newLocation;

            std::cout << "S: Sending reply for earlier request with status code `Created`: name=" << name
                      << ", email=" << email << ", redirected to: " << newLocation << std::endl;

            response.set_status_code(web::http::status_codes::Created);
            request.reply(response);
        }
    }
    catch (const std::exception& e)
    {
        web::json::value json_error{};
        json_error["error_msg"] = web::json::value::string(e.what());

        std::cout << "S: Sending reply for earlier request with status code `InternalError` with body="
                  << json_error.serialize() << std::endl;

        request.reply(web::http::status_codes::InternalError, json_error);
    }
}

void Server::handleGetRequest(const web::http::http_request& request)
{
    // GET /users/123
    //
    // retrieve the user with id = 123 and reply with JSON containing that user's details (status 200)
    // if there is no such user, reply with 404 (NotFound)
    // if any additional data is present but not needed, reply with BadRequest

    // GET /users --- should return list of all users
    try
    {
        auto pathList = web::http::uri::split_path(request.request_uri().path());

        std::cout << "S: Received request at `" << request.request_uri().path() << "`, method = GET\n";

        if (pathList.size() > 2)
        {
            web::json::value jsonReply{};
            jsonReply["error_msg"] = web::json::value::string("unexpected path structure");
            request.reply(web::http::status_codes::BadRequest, jsonReply);
        }
        else if (pathList.size() == 2)
        {
            int id = stoi(pathList[1]);
            auto entity = mStorageEngine->read(id);
            auto jsonReply = web::json::value::array(
                    std::vector<web::json::value>{
                        web::json::value::object(std::vector<std::pair<std::string, web::json::value>>
                            {
                                {
                                    "user", web::json::value::object(std::vector<std::pair<std::string, web::json::value>>
                                    {
                                        {"name", web::json::value::string(entity.getMName())},
                                        {"email", web::json::value::string(entity.getMEmail())},
                                        {"key", web::json::value::string(std::to_string(entity.computeStorageKey()))}
                                    })
                                }
                            })
            });

            std::cout << "S: Sending reply for earlier request with body=" << jsonReply.serialize() << std::endl;
            request.reply(web::http::status_codes::OK, jsonReply);
        }
        else if (pathList.size() == 1)
        {
            auto entities = mStorageEngine->read();
            std::vector<web::json::value> jsonEntities{};
            std::transform(entities.begin(),
                           entities.end(),
                           std::back_inserter(jsonEntities),
                           [](const auto& entityRef)
                           {
                               web::json::value jvalue{};
                               const Entity& entity = entityRef.get();
                               jvalue["user"] = web::json::value::object(
                                       std::vector<std::pair<std::string, web::json::value>>
                                               {
                                                       {"name", web::json::value::string(entity.getMName())},
                                                       {"email", web::json::value::string(entity.getMEmail())},
                                                       {"key", web::json::value::string(std::to_string(entity
                                                                                                 .computeStorageKey()))}
                                               });
                               return jvalue;
                           });
            auto jsonReply = web::json::value::array(jsonEntities);

            std::cout << "S: Sending reply for earlier request, status code OK, body = " << jsonReply.serialize()
                      << std::endl;

            request.reply(web::http::status_codes::OK, jsonReply);
        }
    }
    catch (const web::http::http_exception& e)
    {
        web::json::value jsonReply{};
        jsonReply["error_msg"] = web::json::value::string(e.what());

        std::cout << "S: Sending reply to earlier request, with status code `InternalError`, HTTP error, body="
                  << jsonReply.serialize() << std::endl;

        request.reply(web::http::status_codes::InternalError, jsonReply);
    }
    catch (const std::exception& e)
    {
        web::json::value jsonReply{};
        jsonReply["error_msg"] = web::json::value::string(e.what());

        std::cout << "S: Sending reply to earlier request, with status code `InternalError`, Storage error, body="
                  << jsonReply.serialize() << std::endl;

        request.reply(web::http::status_codes::InternalError, jsonReply);
    }
}

void Server::handlePutRequest(const web::http::http_request& request)
{
    // PUT /users --- reply with 405 (MethodNotAllowed)

    // PUT /users/123?name=MyUserName&email=blabla
    //
    // update the record with id = 123
    // BadRequest, NotFound, InternalServerError, OK, are all possible status codes
    try
    {
        auto pathList = web::http::uri::split_path(request.request_uri().path());
        std::string name{};
        std::string email{};
        web::json::value json_value{};

        std::cout << "S: Received request at `" << request.request_uri().path() << "`, method = PUT\n";

        if (pathList.size() == 1) {
            web::json::value json_error{};
            json_error["error_msg"] = web::json::value::parse("update is not allowed with zero parameters");

            std::cout << "S: Sending reply to earlier request, with status code `MethodNotAllowed`, body="
                      << json_error.serialize() << std::endl;

            request.reply(web::http::status_codes::MethodNotAllowed, json_error);
        }
        if (pathList.size() > 2)
        {
            web::json::value json_error{};
            json_error["error_msg"] = web::json::value::parse("unexpected path structure");

            std::cout << "S: Sending reply to earlier request, with status code `BadRequest`, body="
                      << json_error.serialize() << std::endl;

            request.reply(web::http::status_codes::BadRequest, json_error);
        }
        else
        {
            auto task = request.extract_json();
            task.wait();
            json_value = task.get();
            name = json_value.as_object().at("name").as_string();
            email = json_value.as_object().at("email").as_string();

            int key = stoi(pathList[1]);

            Entity entity{name, email};
            mStorageEngine->update(key, entity);

            std::string newLocation = "/users/" + std::to_string(entity.computeStorageKey());
            auto headers = request.headers();
            headers["Location"] = newLocation;

            std::cout << "S: Sending reply for earlier request with status code `OK`: name=" << name
                      << ", email=" << email << ", redirected to: " << newLocation << std::endl;

            request.reply(web::http::status_codes::OK);
        }
    }
    catch (const std::exception& e)
    {
        web::json::value json_error{};
        json_error["error_msg"] = web::json::value::string(e.what());

        std::cout << "S: Sending reply to earlier request, with status code `InternalError`, body="
                  << json_error.serialize() << std::endl;

        request.reply(web::http::status_codes::InternalError, json_error);
    }
}

void Server::handleDeleteRequest(const web::http::http_request& request)
{
    // DEL /users/123
    //
    // similarly, we can have different replies with different status codes
    try
    {
        auto pathList = web::http::uri::split_path(request.request_uri().path());

        std::cout << "S: Received request at `" << request.request_uri().path() << "`, method = DEL\n";

        if (pathList.size() > 2) {
            web::json::value json_error{};
            json_error["error_msg"] = web::json::value::parse("unexpected path structure");

            std::cout << "S: Sending reply to earlier request, with status code `BadRequest`, body="
                      << json_error.serialize() << std::endl;

            request.reply(web::http::status_codes::BadRequest, json_error);
        }
        else if (pathList.size() == 2){
            int id = stoi(pathList[1]);
            mStorageEngine->delette(id);

            std::cout << "S: Sending reply to earlier request, deleted the entity with id=" << id << std::endl;

            request.reply(web::http::status_codes::OK);
        }
    }
    catch (const std::exception& e)
    {
        web::json::value json_error{};
        json_error["error_msg"] = web::json::value::string(e.what());

        std::cout << "S: Sending reply to earlier request, with status code `InternalError`, body="
                  << json_error.serialize() << std::endl;

        request.reply(web::http::status_codes::InternalError, json_error);
    }
}
