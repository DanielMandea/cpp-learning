#include "Server.h"

Server::Server()
: mListenerList{}
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
}
