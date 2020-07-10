#include <cpprest/http_listener.h>

#include <vector>

namespace web::http { class http_request; }

class Server
{
public:
	Server();

private:
	using ListenerList = std::vector<web::http::experimental::listener::http_listener>;

	ListenerList mListenerList;

	void handleRequest(const web::http::http_request& request);
};
