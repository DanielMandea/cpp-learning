#include <iostream>
#include <cassert>

class MyClass
{
public:
	MyClass(int i) : mCounter{i}, mSecondCounter{-i}
	{

	}

	const int& getFirstCounter() const
	{
		return mCounter;
	}

	const int& getSecondCounter() const
	{
		return mSecondCounter;
	}
	
	void setFirstCounter(int value)
	{
	    mCounter = value;
	}
	
	void setSecondCounter(int value)
	{
	    mSecondCounter = value;
	}

private:
	int mCounter;
	int mSecondCounter;
};

using MyClassFunctionPtr = const int& (MyClass::*)() const;


using MyClassSetFunctionPtr = void (MyClass::*)(int);


const int& doSomething(const MyClass& t, MyClassFunctionPtr p)
{
	return (t.*p)();
}

void doSomethingElse(MyClass* t, MyClassSetFunctionPtr p, int value)
{
    (t->*p)(value);
}



int main()
{        
	MyClass t{10};

	MyClassFunctionPtr ptr1 = &MyClass::getFirstCounter;
	MyClassFunctionPtr ptr2 = &MyClass::getSecondCounter;

	const int& firstCounter = doSomething(t, ptr1);
	const int& secondCounter = doSomething(t, ptr2);

    if (firstCounter != 10)
    {
        std::abort();
    }
    
    if (secondCounter != -10)
    {
        std::abort();
    }
    
	assert(firstCounter == 10);
	assert(secondCounter == -10);

	MyClassSetFunctionPtr ptr3 = &MyClass::setFirstCounter;
	MyClassSetFunctionPtr ptr4 = &MyClass::setSecondCounter;
	
	doSomethingElse(&t, ptr3, 100);
	doSomethingElse(&t, ptr4, -100);

    if (firstCounter != 100)
    {
        abort();
    }
    
    if (secondCounter != -100)
    {
        std::abort();
    }
	
	return 0;
	
}
