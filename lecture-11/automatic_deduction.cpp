#include <iostream>


template <typename T>
void functionName(T input)
{

}


template <typename T>
class Test
{
public:
    Test(const std::initializer_list<T>& list)
    {
        
    }
};


template <typename T>
class AnotherTest
{
public:
    template <typename U>
    AnotherTest(const std::initializer_list<U>& list)
    {
        
    }
};

int main()
{    
    // automatically deduces T = int from the function parameter
    // instantiates a functionName<int>, specializing the functionName template with params T = int
    functionName(5); 
    
    // automatically deduces T = int from the constructor parameter
	Test t{1, 2, 3, 4, 5};
    
    // can no longer deduce T, because it will deduce U from the constructor parameter if the constructor is a template function as well
    // AnotherTest t2{1, 2, 3};

	return 0;
}
