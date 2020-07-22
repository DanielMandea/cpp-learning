#include <cpprest/http_listener.h>
#include <memory>

#include "Entity.h"

namespace web::http { class http_request; }

template <typename EntityT>
class StorageEngineIntf;

class Server
{
public:
	explicit Server(std::unique_ptr<StorageEngineIntf<Entity>> storageEngine);

private:
    web::http::experimental::listener::http_listener mListener;
	std::unique_ptr<StorageEngineIntf<Entity>> mStorageEngine;

	void handlePostRequest(const web::http::http_request& request);

	void handleGetRequest(const web::http::http_request& request);

	void handlePutRequest(const web::http::http_request& request);

	void handleDeleteRequest(const web::http::http_request& request);
};
