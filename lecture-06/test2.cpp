#include <iostream>
#include <vector>


namespace
{
	constexpr double PI = 3.14;
}


class Shape
{
public:
	Shape() : mW{}, mH{}
	{

	}


	Shape(int w, int h) : mW(w), mH(h)
	{
		// aloc memorie
		// deschid socket
		// deschid fisier
		//
	}

	virtual ~Shape() = default;

	virtual int area() const
	{
		return mW * mH;
	}

private:
	int mW;
	int mH;
};


class Circle : public Shape
{
public:
	Circle(int r) : Shape(r, r), mR(r)
	{

	}

	int area() const override
	{
		return PI * mR * mR;
	}

private:
	int mR;
};


class Square : public Shape
{
public:
	Square(int l) : Shape(l, l)
	{

	}
};


int main()
{
	int n;

	std::cin >> n;

	std::vector<Shape*> list{};

	for (int itr = 0; itr < n; ++itr)
	{
		int option;
		std::cin >> option;

		if (1 == option)
		{
			list.push_back(new Circle(10));
		}
		else
		{
			list.push_back(new Square(10));
		}
	}

	for (Shape* s : list)
	{
		std::cout << "shape " << " has area = " << s->area() << "\n\n";
	}

	return 0;
}

/*

int main()
{
	int n;

	std::cin >> n;

	Shape** shape = new Shape*[n];

	for (int itr = 0; itr < n; ++itr)
	{
		int option;
		std::cin >> option;

		if (1 == option)
		{
			shape[itr] = new Circle(10);
		}
		else
		{
			shape[itr] = new Square(10);
		}
	}

	for (int itr = 0; itr < n; ++itr)
	{
		std::cout << "shape #" << itr << " has area = " << shape[itr]->area() << "\n\n";
	}

	return 0;
}

*/