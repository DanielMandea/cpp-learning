#include "Server.h"
#include "Entity.h"
#include "StorageEngineIntf.h"

#include <functional>
#include <unordered_map>

namespace
{
    constexpr const char* HOSTNAME{"http://localhost:8080"};

    using MethodToHandlerMap = std::unordered_map<web::http::method,
                                                  std::function<void(const web::http::http_request&)>>;
}

Server::Server(std::unique_ptr<StorageEngineIntf> storageEngine)
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
        auto queryList = web::http::uri::split_query(request.request_uri().query());

        if ((pathList.size() == 1 && pathList[0] == "/users") || pathList.size() > 2 || queryList.size() != 4) {
            request.reply(web::http::status_codes::BadRequest);
        }
        else {
            std::string name = queryList[1];
            std::string email = queryList[3];
            Entity entity{name, email};
            mStorageEngine->create(entity);
            // redirect to /users/entity.computeStorageKey()
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

        if (pathList.size() > 2) {
            request.reply(web::http::status_codes::BadRequest);
        }
        else if (pathList.size() == 2){
            int id = stoi(pathList[1]);
            mStorageEngine->read(id);
            if () {     // if the read operation has been successfully completed
                request.reply(web::http::status_codes::OK);
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
        auto queryList = web::http::uri::split_path(request.request_uri().query());

        if (pathList.size() == 1)
        {
            request.reply(web::http::status_codes::MethodNotAllowed);
        }
        else if(pathList.size() > 2)
        {
            request.reply(web::http::status_codes::BadRequest);
        }
        else
        {
            std::string name = queryList[1];
            std::string email = queryList[3];
            Entity entity{name, email};
            mStorageEngine->update(entity);
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

        if (pathList.size() > 2) {
            request.reply(web::http::status_codes::BadRequest);
        }
        else if (pathList.size() == 2){
            int id = stoi(pathList[1]);
            mStorageEngine->read(id);
            if () {     // if the read operation has been successfully completed
                request.reply(web::http::status_codes::OK);
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
