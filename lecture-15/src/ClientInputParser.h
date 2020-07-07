#include <memory>
#include <vector>

class Client;

class ClientInputParser
{
public:
	ClientInputParser(int argc, char** argv);

	~ClientInputParser();

	std::vector<std::unique_ptr<Client>> buildClients() const;

private:
	const int mNumberOfClients;
};
