#include <iostream>
#include <functional>

class MyClass
{
public:
	MyClass();

	void doSmth();

	void doSmthElse();


	auto& getValue()
	{
		return mValue;
	}

private:
	int mValue;
};

int p;

const int& f()
{
	return p;
}


int* f2()
{
	return nullptr;
}

int main()
{
	auto& p{f()};

	auto& p2 = f();

	const auto& ref = f();

	auto* ptr = f2();

	return 0;
}
