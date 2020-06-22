#include <thread>
#include <mutex>
#include <condition_variable>

void myFunction()
{

}

int main()
{
	// std::thread myThread{CallableT};

	std::thread myThread{[]()
		{
			// this will run in a new thread
		}};


	myThread.join();

	std::vector<std::thread>

	std::mutex mutex{};

	{
		std::lock_guard lock{mutex};
	}

	return 0;
}