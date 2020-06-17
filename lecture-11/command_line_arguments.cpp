#include <iostream>


int main(int argc, const char* argv[])
{
	std::cout << "Received " << argc << " arguments:";

	for (auto i = 0; i < argc; ++i)
	{
		std::cout << argv[i] << "\n\n";
	}

	return 0;
}
