#include <memory>

template <int T>
void doSomething()
{

}

template <typename T>
class Test
{
public:
	using TPtr = std::unique_ptr<T>;
};


template <>
class Test<int>
{
public:
	using TPtr = std::shared_ptr<int>;
};


int main()
{
	doSomething<5>();

	Test<int>::TPtr myInt = std::make_shared<int>();

	Test<double>::TPtr myDouble = std::make_unique<double>();


	return 0;
}