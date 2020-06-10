#include <iostream>


class Matrix
{

};


int main()
{
	int n;
	int m;

	std::cin >> n >> m;

	Matrix matrix{n, m};

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			int a;
			std::cin >> a;

			matrix.setElementAtPosition(i, j, a);
		}
	}

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			matrix.getElementFromPosition(i, j);
		}
	}

	std::cout << "\n\n";
}
