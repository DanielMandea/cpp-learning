#include <iostream>


#include <vector>


class Matrix
{
public:
	Matrix(int n, int m) : mRows{n}, mColumns{m}, mMat{new int*[mColumns]}
	{
		for (int i = 0; i < mColumns; ++i)
		{
			mMat[i] = new int[mRows];
		}
	}

	~Matrix()
	{
		for (int i = 0; i < mColumns; ++i)
		{
			delete[] mMat[i];
		}

		delete[] mMat;
	}

	Matrix(const Matrix& other) : mRows{other.mRows}, mColumns{other.mColumns}, mMat{new int*[mColumns]}
	{
		for (int i = 0; i < mColumns; ++i)
		{
			mMat[i] = new int[mRows];
		}

		std::memcpy(other.mMat, mMat, m * n * sizeof(int));
	}

	void setElementAtPosition(int line, int col, int element)
	{
		mMat[line][col] = element;
	}

	int getElementFromPosition(int line, int col) const
	{
		return mMat[line][col];
	}


private:
	int mRows;
	int mColumns;



	//std::vector<std::vector<int>> mMat;

	int** mMat;
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
