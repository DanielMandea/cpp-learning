#include <cpprest/http_listener.h>
#include <functional>
#include <iostream>

void handleRequest(const web::http::http_request& request)
{
    std::cout << "received request  " << request.to_string() << "\n\n";
    request.reply(web::http::status_codes::OK, "hello world !!!! \n");
}

int main()
{
    web::http::experimental::listener::http_listener http_listener{"http://localhost:8080"};

    http_listener.support(web::http::methods::GET, std::bind(&handleRequest, std::placeholders::_1));

    http_listener.open().wait();

    while (true)
    {
        // error handling? OS signals interruption handling? anything else?
    }

    // http_listener.close(); ?

    return 0;
}
