#include <iostream>

int main()
{
	UniquePtr<int> p{new int{10}};

	return 0;

	// Questions:
	//
	// 1. Write a template class UniquePtr that behaves as much as a std::unique_ptr as possible. Do not try to be perfect, but support the basics at least.
	//
	// 2. Write a class Test that takes and saves as member a UniquePtr<int>.
}
