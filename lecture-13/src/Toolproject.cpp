#include <iostream>

#include <cpprest/http_client.h>

#include <fstream>


void makeRequest()
{
	web::http::client::http_client client{"http://www.google.com/"};

	web::http::uri_builder builder{"/search"};
	builder.append_query("q", "im feeling lucky");

	auto task1 = client.request(web::http::methods::GET, builder.to_string());

	auto task2 = task1.then([&](const web::http::http_response& response)
	{
		return response.extract_string();
	});

	auto task3 = task2.then([](const std::string& httpResponseBody)
	{
		std::cout << "helloo\n\n" << httpResponseBody << "\n\n";
		std::fstream file{"response.html"};

		file << httpResponseBody;

		file.close();
	});

	auto& task = task3;

    try
    {
        task.wait();
    }
    catch (const std::exception& ex)
    {
		std::cout << "Err: " << ex.what() << "\n\n";
    }
}

int main()
{

	makeRequest();

	return 0;
}
