#include <iostream>

#include <cstring>

#include <string>



void getSomething(std::string& s1, std::string& s2)
{
	s1 = "first return value";
	s2 = "second return value";
}



int main()
{
	int * p = new int[10];
	std::memset(p, 0, 10 * sizeof(p));
	delete[] p;


	int * pp = new int;
	delete pp; 

	{
		double d = 10.7;
		double* ppp = &d;
		// delete ppp;
	}

	int * a = reinterpret_cast<int*>(std::calloc(10 * sizeof(int), 0));


	std::string s1{};
	std::string s2{};

	getSomething(s1, s2);

	if (s1 != "first return value")
	{
		std::cout << "assertion failed !";
		abort();
	}

	return 0;
}