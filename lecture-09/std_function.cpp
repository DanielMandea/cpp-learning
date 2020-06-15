#include <iostream>
#include <functional>

int* f(const int&)
{
	return nullptr;
}

int main()
{

    // std function's template parameter (the one passed between angle brackets < >)
    // is NOT the type of a function pointer.
    
    // a function pointer's type, from our previous lessons: function_return_type (*)(function_parameter_list)
    // in this case, for function "f", it would be: int* (*)(const int&)
    
    // instead the std::function's template parameter has a nicer syntax: return_type (parameter_list)
	std::function<int* (const int&)> myFunction{};

    // also note we still pass address-of "f" to myFunction, but it would work with "f" directly too, while with a function pointer it would not
    //
	myFunction = &f;

	myFunction(5);

	return 0;
}
