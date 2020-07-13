#include <cpprest/http_listener.h>

#include <memory>

namespace web::http { class http_request; }

class StorageEngineIntf;

class Server
{
public:
	explicit Server(std::unique_ptr<StorageEngineIntf> storageEngine);

private:
    web::http::experimental::listener::http_listener mListener;
	std::unique_ptr<StorageEngineIntf> mStorageEngine;

	void handlePostRequest(const web::http::http_request& request);

	void handleGetRequest(const web::http::http_request& request);

	void handlePutRequest(const web::http::http_request& request);

	void handleDeleteRequest(const web::http::http_request& request);
};
