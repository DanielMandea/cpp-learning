#include <iostream>
#include <thread>
#include <chrono>
#include <type_traits>

// this is for the "1s" things to actually mean 1 second
//
using namespace std::chrono;

template<typename T>
T& doSomething()
{
	static T t1{};
	static T t2{};

	MutableRef<T> ref;

	try
	{
		return *ref;
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;

		
		if (duration_cast<seconds>(system_clock::now().time_since_epoch()).count() % 2)
		{
			ref = t1;
		}
		else
		{
			ref = t2;
		}
	}

	return ref;
}

template<typename MutableRefT>
void doSomethingElse(const MutableRefT& ref)
{
	const int& x = ref;
	const int& y = x;
    
    // just to get rid of the unused variable warnings
    //
    static_cast<void>(x);
    static_cast<void>(y);

	// this should NOT work (fail at compilation). why? It's the same as doing int& z = x;
	// it tries to bind a const reference to a non-const one.
	// how? int& z is non-const reference. "ref" is const-qualified, meaning that if I did "ref.getStoredReference()", getStoredReference must be a const member function, otherwise I wouldnt be able
	// to call it on a const object. So if getStoredReference is const, it cannot return something as reference unless the reference is const as well.
	// so wrapping up, I'm trying to get a const reference obtained from "ref" and bind it to a non-const reference int& z. So the code cannot compile.
	//
    // int& z = ref;
}

int main()
{
	// the next line should NOT compile.
	// why? it tries to take, 5 which is a temporary (rvalue reference in C++ slang),
	// and tries to bind that temporary to a MutableRef which should be something that allows me 
	// later to say myMutableRef++ and I expect the 5 to become 6. But since 5 is a temporary, that would not work.
	// So I should employ some kind of mechanism to reject this code at compile time, no reason to even try to do myRef++ because 
	// we already know at the construction time of myRef that the parameter 5 is going to die soon (is temporary).
	//
	// MutableRef<int>{5};

	++doSomething<int>();
	std::this_thread::sleep_for(1s);
	++doSomething<int>();

	std::cout << doSomething<int>() << std::endl;
	std::this_thread::sleep_for(1s);
	std::cout << doSomething<int>() << std::endl;

    int x{};
	doSomethingElse(MutableRef<int>{x});

	return 0;

	// Questions:
    //
    // 1. Write a template class `MutableRef` such that this whole program compiles.
	//
	// 2. By using your `MutableRef` implementation, the 2 lines that are commented through out this skeleton code should cause the compilation to fail if uncommented.
	//
	// 3. When running this program, it should output the following:
    // Invalid access to uninitialized MutableRef
	// Invalid access to uninitialized MutableRef
	// Invalid access to uninitialized MutableRef
	// 1
	// Invalid access to uninitialized MutableRef
	// 1
}
