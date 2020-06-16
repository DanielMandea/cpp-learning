#include <iostream>
#include <functional>

template <typename CallableT>
void doSomething(CallableT&& genericAction)
{
	try
	{
		genericAction();
	}
	catch (...)
	{

	}
}


void f()
{
	std::cout << "Hello world via ptr to function\n\n";
}


class Test
{
public:
	Test()
	{

	}

	void g()
	{            
		std::cout << "Hello world via ptr to member function\n\n";
	}
   
};


int sumFourNumbers(int a, int b, int c, int d)
{
    return a + b + c + d;
}


int main()
{
	doSomething(&f);
    
    Test t{};
    
    doSomething(std::bind(&Test::g, t));
    
    auto sumTwoNumbers = std::bind(&sumFourNumbers, std::placeholders::_1, 100, std::placeholders::_2, 500);
    
    auto sum = sumTwoNumbers(100, 500);
    
    std::cout << sum << "\n\n";
    
	return 0;
}
