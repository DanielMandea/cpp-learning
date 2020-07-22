#include "Server.h"
#include "StorageEngineIntf.h"

#include <cpprest/json.h>
#include <functional>
#include <string>
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
    // POST /users
    //
    // create a new user with the data retrieved from the HTTP URI query,
    // then reply with 201 (Created) and redirect to /users/123 (123 = ID assigned to the freshly created user).
    // if there is missing data in the URI query, reply with BadRequest
    // if any internal error came up on the server, reply with InternalServerError

    try
    {
        auto pathList = web::http::uri::split_path(request.request_uri().path());
        std::string name{};
        std::string email{};
        web::json::value jvalue;

        std::cout << request.to_string() << "\n\n";

        if (pathList.size() == 1 && pathList.at(0) == "users")
        {
            auto task = request.extract_json();
            task.wait();
            jvalue = task.get();

            name = jvalue.as_object().at("name").as_string();
            email = jvalue.as_object().at("email").as_string();
            Entity entity {name, email};
            mStorageEngine->create(entity);

            //redirect to /users/123 (123 = ID assigned to the freshly created user).
            std::string newLocation = "/users/" + std::to_string(entity.computeStorageKey());
            auto headers = request.headers();
            headers["Location"] = newLocation;
            request.reply(web::http::status_codes::Created);

        }
        else if ((pathList.size() == 1 && pathList.at(0) != "users") || pathList.size() > 1)
        {
            request.reply(web::http::status_codes::BadRequest);
        }
    }
    catch (const std::exception& e)
    {
        std::cout<<"EXEPTION\n\n"<<e.what()<<"\n\n";
        request.reply(web::http::status_codes::InternalError);
    }


    // verify and process the path elements and any URI-Query data provided.
    //
    // valid example requests:
    //
    // the line below will probably no longer fulfill our replying needs as they got more complex.
    //
    // request.reply(handler() ? web::http::status_codes::OK : web::http::status_codes::BadRequest);
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
        web::json::value reply{};

        if (pathList.size() == 1 && pathList.at(0) == "/users")
        {
            auto entities = mStorageEngine->read();
            int it = 0;
            for (const auto& r_entity : entities)
            {
                const auto& entity = r_entity.get();
                std::string user = "user" + std::to_string(it++);
                reply[user]["name"] = web::json::value::string(entity.getName());
                reply[user]["email"] = web::json::value::string(entity.getEmail());
            }
            if (!entities.empty())
            {
                request.reply(web::http::status_codes::OK, reply);
            }
            else
            {
                request.reply(web::http::status_codes::NotFound);
            }
        }
        if (pathList.size() > 2)
        {
            request.reply(web::http::status_codes::BadRequest);
        }
        if (pathList.size() == 2)
        {
            int key = stoi(pathList.at(1));
            auto entity = mStorageEngine->read(key);
            reply["user"]["name"] = web::json::value::string(entity.getName());
            reply["user"]["email"] = web::json::value::string(entity.getEmail());
            if (entity.getName().empty() || entity.getEmail().empty())
            {
                request.reply(web::http::status_codes::NotFound);
            }
            else
            {
                request.reply(web::http::status_codes::OK, reply);
            }
        }
    }
    catch (const std::exception& e)
    {
        request.reply(web::http::status_codes::InternalError);
    }
}

void Server::handlePutRequest(const web::http::http_request& request) {
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
        web::json::value jvalue;

        if (pathList.size() == 1 && pathList.at(0) == "/users") {
            request.reply(web::http::status_codes::MethodNotAllowed);
        }
        if (pathList.size() > 2) {
            request.reply(web::http::status_codes::BadRequest);
        }
        if (pathList.size() == 2) {

            auto task = request.extract_json();
            task.wait();
            jvalue = task.get();

            name = jvalue.as_object().at("name").as_string();
            email = jvalue.as_object().at("email").as_string();
            int key = stoi(pathList[1]);
            Entity entity {name, email};
            mStorageEngine->update(key, entity);
            request.reply(web::http::status_codes::OK);
        }
    }
    catch (const std::exception& e)
    {
        request.reply(web::http::status_codes::InternalError);
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

        if (pathList.size() == 1 && pathList.at(0) == "/users")
        {
            request.reply(web::http::status_codes::MethodNotAllowed);
        }
        if (pathList.size() > 2)
        {
            request.reply(web::http::status_codes::BadRequest);
        }
        if (pathList.size() == 2)
        {
            int key = stoi(pathList.at(1));
            mStorageEngine -> delette(key);
            request.reply(web::http::status_codes::OK);
        }
    }
    catch (const std::exception& e)
    {
        request.reply(web::http::status_codes::InternalError);
    }
}
