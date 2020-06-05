

// we use #include <FILE> when specifying system headers (standard library or system headers like <windows.h>)
#include <iostream>

// we use #include "FILE" when specifying our own custom headers.
#include "Rectangle.hxx"

#include "Square.hxx"

// the <> vs "" is a convention, but it also does something: with " ", compiler will look in the current folder, it expects a path... #include "certain/path/to/file/Rectangle.hxx"
//                                                           with <>, compiler will look in the PATH environment variable as well.




int main()
{
	// remember how we used {} for zero-initialization? We can use it for declaring variables as well. It's a good habit to pick up. Rectangle r; would work too.
	//
	Rectangle r1{};

	Rectangle r2{5, 5};

	Rectangle r3{r1};


	Rectangle r4{};


	r4 = r3;


	r1.set(10, 10);

	std::cout << "r1 area = " << r1.getArea() << ", r2 area = " << r2.getArea() << "\n\n";

	// calling a static function without any instance
	Rectangle::standAloneFunction();

	// or calling it with an instance, but to be honest this makes no sense, it's pointless 
	r1.standAloneFunction();


	std::cout << "\n\n\n";

	Square square{};

	std::cout << "\n\n\n";



}
