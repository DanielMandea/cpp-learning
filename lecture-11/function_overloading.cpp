#include <iostream>


int foo(int a, int b)
{
	return a + b;
}

int foo(int a, int b, int c)
{
	return a + b + c;
}

int main()
{
	std::cout << foo(10, 20) << "\n\n";

	std::cout << foo(10, 10, 10) << "\n\n";

	return 0;
}
