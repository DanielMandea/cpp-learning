#include <iostream>

void f()
{
	std::cout << "Hello";
}

int* allocateMemory(const char*, int**)
{
	return nullptr;
}

int main()
{
	// define a pointer to a function that takes no parameters and returns void

	// 1st: define this custom Type via "typedef" keyword
	typedef void (*PointerToVoidFunction)();

	// 2nd: define an equivalent custom Type via "using" keyword
	using AnotherPointerToVoidFunction = void (*)();

	// now use 1st and 2nd custom types to declare some variables of those specific types
	// see how the types are equivalent because both variables can be initialized with address-of "f"
	PointerToVoidFunction myPtr = &f;
	AnotherPointerToVoidFunction myPtr2 = &f;

	// call f via myPtr
	(*myPtr)();

	// call f via myPtr2
	(*myPtr2)();

	// the regular, non-function-to-pointer syntax for typedefs
	// see how it is different from the one we defined 1st custom type with -- typedef void (*MyNameForTheCustomType)()
	typedef double Height;
	Height myOwnHeight{};
    myOwnHeight = 182.5;

	// prefer the "using" syntax, it's C++11 and modern and can be used to define template custom types too!
	// lets use the using syntax for another pointer to function type, this time more complex
	using PointerToComplexFunction = int* (*)(const char*, int**);

	// use this type to initialize a variable 
	PointerToComplexFunction ptrToAllocateMemory = &allocateMemory;

	// call the allocateMemory function using the "ptrToAllocateMemory" variable
	// pass nullptr for the 2 parameters (const char* and int**)
	// receive the return value result in another variable of type int*
	int* returnValueOfAllocateMemory = (*ptrToAllocateMemory)(nullptr, nullptr);

	return 0;
}
