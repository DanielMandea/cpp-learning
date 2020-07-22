#include <cstddef>
#include <string>
#include <utility>

int main()
{
	List<std::pair<std::string, std::size_t>> myList{};

	myList.insert(std::make_pair("dog", 0));
	myList.insert(std::make_pair("cpp-training-test", 0));

	myList.applyToAll([](auto& element)
	{
		element.second = element.first.length();
	});

	myList.applyToAll([](auto& element))
	{
		std::cout << element.second << ", ";
	}

	return 0;

	// Questions:
    //
    // 1. Write a template class `List` such that this whole program compiles.
	//
	// 2. When running this program, it should output the following: 3, 17
	//
	// 3. Write client code at line 24 that achieves the same effect but does not use a lambda expression. Instead, the logic of the lambda expression should go into a member function of some class X.
	// (you still need to do myList.applyToAll(somethingHere) in the client code).
}
