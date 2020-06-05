#include "Rectangle.hxx"

// anonymous namespace, e.g. it has no name (as opposed to namespace mySpecialNamespace { })
namespace
{
	int addNumbers(int a, int b)
	{
		return a + b;
	}
}



// below we have the implementation for all the methods declared in the Rectangle class, in file Rectangle.hxx

Rectangle::Rectangle() : mWidth{}, mHeight{}, mArea{100}
{
	// mDataMember{} == C++11 syntax == "zero initialization" == mDataMember's memory will always be zeroed.
	// Other types of initialisation, like mDataMember(), do not guarantee zeroed memory, e.g. we need to specify it by saying mDataMember(0)

	// the portion between ")" and "{" is called "member initializer list" and has the syntax Class::Class() : member1, member2, member3...

	// regardless of whether we do use the initializer list or not, that step will always be performed

	// if in this case for example we did not specify the " : mWidth{}, mHeight{} " at all, for example if our constructor had looked like
	// Rectangle::Rectangle() { }, then the compiler would automatically make our member initializer look like this: Rectangle::Rectangle() : mWidth(), mHeight()
	// which is not what we want because it does NOT assure us of zeroed memory, e.g. we could wake up in the body of the constructor that our mWidth variable has value 6836392745

	// that is why we always:
	// 1. use member initializer lists in constructors, never skip them, even if compiler lets us
	// 2. use C++11 data{} syntax, not data(), even if we pass any values between { } or not, like when copying from parameters: Rectangle::Rectangle(int width, int height) : mWidth{width}
	//                                                                                         or even when we hardcode the value, like this: mWidth{100}
	//
}

Rectangle::Rectangle(int width, int height) : mWidth{width}, mHeight{height}, mArea{100}
{
	// addNumbers is a function in an anonymous namespace declared & defined in this Rectangle.cpp file. Thus, it may be used here and ONLY here. It is not visible in any other file.
	//
	int sum = addNumbers(mWidth, mHeight);
}

Rectangle::Rectangle(const Rectangle& object) : mWidth{object.mWidth}, mHeight{object.mHeight}
{

}

Rectangle& Rectangle::operator=(const Rectangle& object)
{
	mWidth = object.mWidth;
	mHeight = object.mHeight;

	return *this;
}

int Rectangle::getArea() const
{
	// if we were to call set(10, 10), a non-const member function, compiler would forbit it
	// so const member functions may call ONLY other const member functions

	return mWidth * mHeight;
}

void Rectangle::set(int width, int height)
{
	// this is a non-const member function, therefore we can call from it const and non-const member functions

	mWidth = width;
	mHeight = height;
}

// see how I did not not to include this until up this point and I can include it here? It works!
// Please don't do this, always have all the includes at the top of the file.
#include <iostream>

// see how we tell the compiler this is a static function only at its declaration point, but not here in the implementation
//
void Rectangle::standAloneFunction()
{
	std::cout << "\n\n I am a static function, you probably called me without even having an instance of the Rectangle class ! \n\n";

	// because this is a static function, if we had tried to access any non-static data member of the class, the compiler would issue an error

	// we can of course call other static functions or access static data members of the class
}
