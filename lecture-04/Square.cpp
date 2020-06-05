#include "Square.hxx"

#include <iostream>

Square::Square() : Rectangle()
{
	std::cout << "my `inherited area` is = " << mArea;
}