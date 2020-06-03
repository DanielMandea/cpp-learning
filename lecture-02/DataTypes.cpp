#include <iostream>
#include <limits>


class MyClass
{
public:
	enum class Colors
	{
		RED,
		BLUE,
		YELLOW
	};
};



namespace myStd
{
	template <typename T>
	class numeric_limits
	{
	public:
		static T max()
		{
			return 18446744073709551615U;
		}
	};
}



int main()
{
	char myChar = 'a';

	int myInt = 100;

	short myShort = 10;

	unsigned long myLong = 64;

	MyClass::Colors myColor = MyClass::Colors::RED;



	std::cout << "short max value = " << std::numeric_limits<unsigned short>::max() << "\n\n";
	std::cout << "int max value = " << std::numeric_limits<unsigned int>::max() << "\n\n";
	
	std::cout << "long max value = " << myStd::numeric_limits<unsigned long>::max() << "\n\n";

	return 0;
}
