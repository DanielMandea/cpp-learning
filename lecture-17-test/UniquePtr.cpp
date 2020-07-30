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
	
	UniquePtr(const T& value)
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
    template <typename... Args> 										
	static UniquePtr<T> makeUnique(Args&& ...args)
	{
		return UniquePtr<T>(new T(std::forward<Args>(args)...));		// std::forward = forwards as lvalue or as rvalue, depending on Args
	}

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

	//dangling reference
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

	A()
	: mAmember1{},
	  mAmember2{},
	  mAmember3{}

	{

	}

	A(int a, int b, std::string c)
	: mAmember1{a},
	  mAmember2{b},
	  mAmember3{c}
	{

	}
    void show() 
    { 
        std::cout << "A::show() \n" << mAmember1<< " , "<<mAmember2 <<" , "<<mAmember3 <<"\n"; 
    }
private:
	int mAmember1;
	int mAmember2;
	std::string mAmember3;
};

void testForMake()
{
	UniquePtr<A> ptr = UniquePtr<A>::makeUnique<A>(A(10,12, "String test"));
	ptr->show();

}


int main()
{
	// testForMake();

	// 	UniquePtr<int> p{new int{10}};

	// UniquePtr<int> p1; //starts as nullptr

	// 	std::cout <<"p is " << (p.isNull() ? "null\n" : "not-null\n");
	// std::cout <<"p1 is " << (p1.isNull() ? "null\n" : "not-null\n");
	
	// 	UniquePtr<A> ptrA{new A};
	// 	(*ptrA).show();
	
	// 	UniquePtr<A> ptrA1;
	//  (*ptrA1).show();


	// UniquePtr<int> ptrA{11};
	// UniquePtr<int> ptrA2{12};

	// auto ptr2 = UniquePtr<int>{};
	// std::cout<<  "ptr2 is " << (ptr2.isNull() ? "null\n" : "not-null => " +std::to_string(*ptr2) +"\n");


	// Test t{ptrA};
	// std::cout<< t.getTest(); //prints 11 

  
    UniquePtr<int> ptrA = UniquePtr<int>::makeUnique<>(10);
    std::cout<<  "ptrA is " << (ptrA.isNull() ? "null\n" : "not-null => " +std::to_string(*ptrA) +"\n");

	(1): nu pot face assignarea de mai jos
	auto ptrA5 = ptrA;

 	(2): trebuie sa mearga

	auto ptrB = std::move(ptrA);
	std::cout<<  "ptrB is " << (ptrB.isNull() ? "null\n" : "not-null => " +std::to_string(*ptrB) +"\n");

 	// (3): release 
	ptrA = nullptr;

 	// (4): sa fie bool
	assert(ptrA == nullptr);
    std::cout << "Execution continues past the first assert\n";

 	// (5): sa functioneze mai jos
 	assert(!ptrA);
    std::cout << "Execution continues past the second assert\n";

 	//(6):
	UniquePtr<int> ptrA3{};
	std::cout<<  "ptrA3 is " << (ptrA3.isNull() ? "null\n" : "not-null => " +std::to_string(*ptrA3) +"\n");
	UniquePtr<int> ptrA4{nullptr};
	std::cout<<  "ptrA4 is " << (ptrA4.isNull() ? "null\n" : "not-null => " +std::to_string(*ptrA4) +"\n");

	return 0;

	// Questions:
	
	// 1. Write a template class UniquePtr that behaves as much as a std::unique_ptr as possible. Do not try to be perfect, but support the basics at least.
	
	// 2. Write a class Test that takes and saves as member a UniquePtr<int>.
}






