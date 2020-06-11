#include <iostream>


#include <vector>


class Matrix
{
public:
	// implement this class
};




int main()
{
	int n;
	int m;

	std::cin >> n >> m;

	Matrix matrix{n, m};

	Matrix matrix2{matrix};

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			int a;
			std::cin >> a;

			matrix.setElementAtPosition(i, j, a);
		}
	}

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			matrix.getElementFromPosition(i, j);
		}
	}

	std::cout << "\n\n";
}
