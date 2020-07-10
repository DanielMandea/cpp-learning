#include <cpprest/http_listener.h>

#include <memory>
#include <vector>

namespace web::http { class http_request; }

class StorageEngineIntf;

class Server
{
public:
	Server(std::unique_ptr<StorageEngineIntf> storageEngine);

private:
	using ListenerList = std::vector<web::http::experimental::listener::http_listener>;

	ListenerList mListenerList;

	std::unique_ptr<StorageEngineIntf> mStorageEngine{};

	void handleRequest(const web::http::http_request& request);
};
