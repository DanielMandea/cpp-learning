#include "Server.h"
#include "StorageEngineIntf.h"

Server::Server(std::unique_ptr<StorageEngineIntf> storageEngine)
: mListenerList{},
  mStorageEngine{std::move(storageEngine)}
{
	mListenerList.emplace_back("http://localhost:8080/create");
	mListenerList.emplace_back("http://localhost:8080/read");
	mListenerList.emplace_back("http://localhost:8080/update");
	mListenerList.emplace_back("http://localhost:8080/delete");

	for (auto& listener : mListenerList)
	{
		listener.support([this](const web::http::http_request& request)
		{
			handleRequest(request);
		});

		listener.open();
	}
}

void Server::handleRequest(const web::http::http_request& request)
{
	std::cout << "\n request_uri = " << request.request_uri().path() << "\n\n";

	request.reply(web::http::status_codes::OK);

    std::string operation = request.request_uri().path().substr(1);
    std::cout << request.to_string() << "\n\n";

    if (request.method() == web::http::methods::GET)
    {
        mStorageEngine->read();
    }
    else if (request.method() == web::http::methods::POST)
    {
        auto path = request.request_uri().path().substr(1);
        if (path == "create")
        {
            mStorageEngine->create();
        }
        else if (path == "update")
        {
            mStorageEngine->update();
        }
    }
    else if (request.method() == web::http::methods::DEL)
    {
        mStorageEngine->delette();
    }
}
