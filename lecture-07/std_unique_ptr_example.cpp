#include <iostream>
#include <memory>


class Test
{
public:
	Test() : mPtr{std::make_unique<int>(123)}
	{

	}

	std::unique_ptr<int> mPtr;
};

int main()
{

	Test t{};

	std::cout << *t.mPtr << "\n";

	return 0;
}