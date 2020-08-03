#include <memory>
#include <queue>

class ClientAction;

class Client
{
public:
	Client();

	~Client();

	void addAction(std::unique_ptr<ClientAction> clientAction);

	void executeActions();

private:
	std::queue<std::unique_ptr<ClientAction>> mActions;
};
