#include <iostream>


void modifyValue(int** value)
{
	*value = new int;
}


int main()
{
	int** myPtrToInt = nullptr;

	modifyValue(&myPtrToInt);

	if (myPtrToInt)
	{

	}
	else
	{
		std::cout << "invalid pointer \n";
	}


	return 0;
}
