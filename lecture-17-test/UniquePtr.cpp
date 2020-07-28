#include <cassert>
#include <iostream>
#include <memory>
#include <string>

template <typename T>
class UniquePtr
{
public:
	UniquePtr()
	:mPtr{new T}
	{

	}
	
	UniquePtr(T value)
	: mPtr{new T{value}}
	{
	    
	}

	explicit UniquePtr(T* ptr) 
	: mPtr{ptr}
	{

	}

    // UniquePtr()
    // : mPtr{nullptr}
    // {
        
    // }

    UniquePtr(std::nullptr_t)
    : mPtr{nullptr}
    {
        
    }

    UniquePtr& operator=(std::nullptr_t)
    {
        T* result = nullptr;
        std::swap(result, mPtr);
        delete result;
        return *this;
    }

	~UniquePtr()
	{
		delete mPtr;
	}
	
	static UniquePtr<T> makeUnique(UniquePtr<T>& a)
	{
		UniquePtr<T> ret{a.mPtr};
 		delete a.mPtr;
		a.mPtr = nullptr;
		return ret;
	}
	
	//move constructor 
// 	UniquePtr(UniquePtr<T>&& a)
// 	: mPtr{a.mPtr}
// 	{
// 		a.mPtr = nullptr;
// 	}
	
	T& operator*() 
	{ 
		return *mPtr; 
	}

	const T& operator*() const 
	{
		return *mPtr;
	}

	T* operator->() const 
	{
		return mPtr;
	}

	T& release()
	{
		T& ret = *mPtr;
		mPtr = nullptr;
		return ret;
	}
	
	T* get()
	{
	    return mPtr;
	}
	
	bool isNull()
	{
	    return mPtr == nullptr;
	}
	
	bool operator ==(std::nullptr_t)
	{
	   return mPtr == nullptr;
	}

	explicit operator bool() const 
	{
	    return mPtr;
	}

private:
	T* mPtr;
};

template <typename T>
class Test
{
public:
	Test()
	: mTest{}
	{

	}

	Test(UniquePtr<T> value)
	: mTest{value}
	{

	}
	
	T& getTest()
	{
		return *mTest;
	}

private:
	UniquePtr<T> mTest;
};

class A 
{ 
public: 
    void show() 
    { 
        std::cout << "A::show() \n"; 
    }
};


int main()
{
	// 	UniquePtr<int> p{new int{10}};

	// UniquePtr<int> p1; //starts as nullptr

	// 	std::cout <<"p is " << (p.isNull() ? "null\n" : "not-null\n");
	// std::cout <<"p1 is " << (p1.isNull() ? "null\n" : "not-null\n");
	
	// 	UniquePtr<A> ptrA{new A};
	// 	(*ptrA).show();
	
	// 	UniquePtr<A> ptrA1;
	//  (*ptrA1).show();


	UniquePtr<int> ptrA{11};
	// UniquePtr<int> ptrA2{12};

	// auto ptr2 = UniquePtr<int>{};
	std::cout<<  "ptr2 is " << (ptr2.isNull() ? "null\n" : "not-null => " +std::to_string(*ptr2) +"\n");


	// Test<int> t{ptrA};
	// std::cout<< t.getTest(); //prints 11 


    UniquePtr<int> ptrA1 = UniquePtr<int>::makeUnique(ptrA);
    
    std::cout<<  "ptrA is " << (ptrA.isNull() ? "null\n" : "not-null => " +std::to_string(*ptrA) +"\n");

	// (1): nu pot face assignarea de mai jos
	auto ptrA5 = ptrA;

 	// (2): trebuie sa mearga

	auto ptrB = std::move(ptrA2);
	std::cout<<  "ptrB is " << (ptrB.isNull() ? "null\n" : "not-null => " +std::to_string(*ptrB) +"\n");

 	// (3): release 
	ptrA = nullptr;

 	//(4): sa fie bool
	assert(ptrA1 == nullptr);
    std::cout << "Execution continues past the first assert\n";

 	//(5): sa functioneze mai jos
 	assert(!ptrA1);
    std::cout << "Execution continues past the second assert\n";

 	//(6):
	UniquePtr<int> ptrA3{};
	std::cout<<  "ptrA3 is " << (ptrA3.isNull() ? "null\n" : "not-null => " +std::to_string(*ptrA3) +"\n");
	UniquePtr<int> ptrA4{nullptr};
	std::cout<<  "ptrA4 is " << (ptrA4.isNull() ? "null\n" : "not-null => " +std::to_string(*ptrA4) +"\n");

	return 0;

	// Questions:
	//
	// 1. Write a template class UniquePtr that behaves as much as a std::unique_ptr as possible. Do not try to be perfect, but support the basics at least.
	//
	// 2. Write a class Test that takes and saves as member a UniquePtr<int>.
}






