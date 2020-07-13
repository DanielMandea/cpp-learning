#include "Server.h"
#include "StorageEngineIntf.h"

Server::Server(std::unique_ptr<StorageEngineIntf> storageEngine)
: mListenerList{},
  mStorageEngine{std::move(storageEngine)}
{
	mListenerList.emplace_back("http://localhost:8080/users");
//    mListenerList.emplace_back("http://localhost:8080/read");
//    mListenerList.emplace_back("http://localhost:8080/update");
//    mListenerList.emplace_back("http://localhost:8080/delete");

	for (auto& listener : mListenerList)
	{
		listener.support(web::http::methods::POST,
		                 std::bind(&Server::handleRequest,
		                           this,
		                           std::placeholders::_1,
		                           std::bind(&StorageEngineIntf::create, mStorageEngine.get())));
//		listener.support(web::http::methods::GET, std::bind(&Server::handleRequest, this, std::placeholders::_1));
//		listener.support(web::http::methods::PUT, std::bind(&Server::handleRequest, this, std::placeholders::_1));
//		listener.support(web::http::methods::DEL, std::bind(&Server::handleRequest, this, std::placeholders::_1));

		listener.open();
	}
}

void Server::handleRequest(const web::http::http_request& request, const std::function<bool()>& handler)
{
    request.reply(handler() ? web::http::status_codes::OK : web::http::status_codes::BadRequest);
}
