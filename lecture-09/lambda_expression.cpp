#include <iostream>
#include <functional>


char f(int)
{
    return 'A';
}


int main()
{
	std::function<char(int)> myFunction{};

    try
    {
        myFunction(5);
    }
    catch (const std::bad_function_call& ex)
    {
        
    }
    catch (const std::exception& ex)
    {
       
    }

	myFunction = [&](auto value) -> char
	{
		switch (value)
		{
			case 0:
			{
				return 'A';
			}

			default:
			{
				return 'B';
			}
		}
	};

	myFunction(100);

	myFunction = f;

	myFunction(10);

	return 0;
}
