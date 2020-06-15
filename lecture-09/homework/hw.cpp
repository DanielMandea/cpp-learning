#include <iostream>
#include <functional>

template <typename T>
void doSomething(T&& genericAction)
{
	try
	{
		genericAction();
	}
	catch (...)
	{

	}
}

int main()
{
	// ptr to function
	// create necessary infrastructure
	// auto ptrToFunction = value;
	//
	doSomething(ptrToFunction);

	// ptr to member function
	// create necessary class and infrastructure if needed
	// auto pointerToMemberFunction = value;
	//
	doSomething(pointerToMemberFunction);

	// std function
	// type stdFunction{value};
	// or: type stdFunction = value;
	// or using auto instead of type;
	//
	doSomething(stdFunction);

	// lambda expression
	//
	doSomething([]()
	{
		std::cout << "lambda expression body\n\n";
	});

	return 0;
}
