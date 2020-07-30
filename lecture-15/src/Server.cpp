#include "Server.h"
#include "StorageEngineIntf.h"

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

        if ((pathList.size() == 1 && pathList[0] != "users") || pathList.size() > 1) {
            request.reply(web::http::status_codes::BadRequest);
        }
        else {
            auto task = request.extract_json();
            task.wait();
            json_value = task.get();
            name = json_value.as_object().at("name").as_string();
            email = json_value.as_object().at("email").as_string();

            std::cout << "create\n";
            std::cout << "name: " << name << "; email: " << email << std::endl;

            Entity entity{name, email};
            mStorageEngine->create(entity);
            std::string newLocation = "/users/" + std::to_string(entity.computeStorageKey());
            auto headers = request.headers();
            headers["Location"] = newLocation;
            std::cout << "redirect location: " << headers["Location"] << "\n";
            request.reply(web::http::status_codes::Created);
        }
    }
    catch (const std::exception& e)
    {
        request.reply(web::http::status_codes::InternalError);
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
    try {
        auto pathList = web::http::uri::split_path(request.request_uri().path());
        web::json::value json_reply{};

        std::cout << "read\n";

        if (pathList.size() > 2) {
            request.reply(web::http::status_codes::BadRequest);
        }
        else if (pathList.size() == 2) {
            int id = stoi(pathList[1]);
            auto entity = mStorageEngine->read(id);
            json_reply["user"]["name"] = web::json::value::string(entity.getMName());
            json_reply["user"]["email"] = web::json::value::string(entity.getMEmail());
            if (!entity.getMName().empty()) {     // check if the read operation has been successfully completed
                request.reply(web::http::status_codes::OK, json_reply);
            } else {
                request.reply(web::http::status_codes::NotFound);
            }
        }
        else if (pathList.size() == 1) {
            auto entities = mStorageEngine->read();
            int count = 0;
            for (const auto& ref_entity : entities) {
                const auto& entity = ref_entity.get();
                std::string user = "user" + std::to_string(count++);
                json_reply[user]["name"] = web::json::value::string(entity.getMName());
                json_reply[user]["email"] = web::json::value::string(entity.getMEmail());
            }
            if (entities.size() > 0) {
                request.reply(web::http::status_codes::OK, json_reply);
            } else {
                request.reply(web::http::status_codes::NotFound);
            }
        }
    }
    catch (const std::exception& e)
    {
        request.reply(web::http::status_codes::InternalError);
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

        std::cout << "update\n";

        if (pathList.size() == 1) {
            request.reply(web::http::status_codes::MethodNotAllowed);
        }
        if (pathList.size() > 2)
        {
            request.reply(web::http::status_codes::BadRequest);
        }
        else
        {
            auto task = request.extract_json();
            task.wait();
            json_value = task.get();
            name = json_value.as_object().at("name").as_string();
            email = json_value.as_object().at("email").as_string();

            std::cout << "name: " << name << "; email: " << email << std::endl;

            int key = stoi(pathList[1]);

            Entity entity{name, email};
            mStorageEngine->update(key, entity);

            std::string newLocation = "/users/" + std::to_string(entity.computeStorageKey());
            auto headers = request.headers();
            headers["Location"] = newLocation;
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

        std::cout << "delete\n";

        if (pathList.size() > 2) {
            request.reply(web::http::status_codes::BadRequest);
        }
        else if (pathList.size() == 2){
            int id = stoi(pathList[1]);
            mStorageEngine->delette(id);
            request.reply(web::http::status_codes::OK);
        }
    }
    catch (const std::exception& e)
    {
        request.reply(web::http::status_codes::InternalError);
    }
}
