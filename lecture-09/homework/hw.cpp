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


void f()
{
	std::cout << "Hello world via ptr to function\n\n";
}


class Test
{
public:
	Test()
	{

	}

	void g()
	{
		std::cout << "Hello world via ptr to member function\n\n";
	}
};



int main()
{
	using PtrToFunction = void (*)();
	PtrToFunction v1 = &f; // using "f" directly hints to the compiler that it needs to add the "address-of" -- &, but technically correct is to add the & ourselves.
	doSomething(v1); // doSomething(&f) or doSomething(f) would be just as fine, because why need a separate variable?


	using PtrToMemFunction = void (Test::*)();
	PtrToMemFunction v2 = &Test::g;
	Test t{};
	doSomething(std::bind(&Test::g, t)); // here we need to pass to doSomething the function AND the instance on which it will call that function, so we use std::bind to tie these two together in a single argument

	// lambda expression
	//
	doSomething([]() // easiest from the hood, just pass directly a lambda expression to doSomething
	{
		std::cout << "lambda expression body\n\n";
	});

	return 0;
}
