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
                                                           }
                                                       })
    {
        mListener.support(requestMethod, requestHandler);
    }

    mListener.open().wait();
}

void Server::handlePostRequest(const web::http::http_request& request)
{
    mStorageEngine->create();


    auto pathList = web::http::uri::split_path(request.request_uri().path());

    // verify and process the path elements and any URI-Query data provided.
    //
    // valid example requests:
    //

    // POST /users?name=MyUserName&email=useremailatgmail.com
    //
    // create a new user with the data retrieved from the HTTP URI query,
    // then reply with 201 (Created) and redirect to /users/123 (123 = ID assigned to the freshly created user).
    // if there is missing data in the URI query, reply with BadRequest
    // if any internal error came up on the server, reply with InternalServerError

    // GET /users/123
    //
    // retrieve the user with id = 123 and reply with JSON containing that user's details (status 200)
    // if there is no such user, reply with 404 (NotFound)
    // if any additional data is present but not needed, reply with BadRequest

    // GET /users --- should return list of all users

    // PUT /users --- reply with 405 (MethodNotAllowed)

    // PUT /users/123?name=MyUserName&email=blabla
    //
    // update the record with id = 123
    // BadRequest, NotFound, InternalServerError, OK, are all possible status codes

    // DEL /users/123
    //
    // similarly, we can have different replies with different status codes


    // the line below will probably no longer fulfill our replying needs as they got more complex.
    //
    // request.reply(handler() ? web::http::status_codes::OK : web::http::status_codes::BadRequest);
}

void Server::handleGetRequest(const web::http::http_request& request)
{
    mStorageEngine->read();
}

void Server::handlePutRequest(const web::http::http_request& request)
{
    mStorageEngine->update();
}

void Server::handleDeleteRequest(const web::http::http_request& request)
{
    mStorageEngine->delette();
}
