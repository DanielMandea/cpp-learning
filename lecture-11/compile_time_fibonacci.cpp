#include <iostream>


template <int N>
struct Fibonacci
{
	static constexpr int Value = Fibonacci<N - 1>::Value + Fibonacci<N - 2>::Value;
};


template <>
struct Fibonacci<2>
{
	static constexpr int Value = 1;
};


template <>
struct Fibonacci<1>
{
	static constexpr int Value = 1;
};


int main()
{
	// this is just like std::cout << 55 << "\n\n"; 
	//
	// meaning that the value is computed when compiling the code, e.g. when executing it is an already known constant = 55
	//
	std::cout << Fibonacci<10>::Value << "\n\n";

	return 0;
}
