#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

template <typename T>
class Node
{
public:
	Node(const T& value)
	: value{value},
	  next{nullptr}
	{

	}

	T value;
	Node<T>* next;
};


template <typename T>
class List
{
public:
	Node<T>* mFirst;
	int count;

	List()
	: mFirst{nullptr},
	  count{0}
	{

	}

	// template <typename F>
	// void applyToAll(F&& function)
	void applyToAll(const std::function<void(T&)>& function) const
	{
		if (count != 0)
		{
			Node<T>* node = mFirst;
		
			while (node != nullptr) 
			{
				function(node->value);
				node = node->next;
			}
		}
	}


	void insert(const T& value)
	{	
		Node<T>* temp = new Node<T>{value};

		if (mFirst == nullptr)
		{
			mFirst = temp;
			count = 1;
			return;
		}

		Node<T>* node = mFirst;
		while (node->next != nullptr)
		{
			node = node->next;
		}
		node->next = temp;
		count++;
	}

	void deleteValue(int value)
	{
		Node<T>* node = mFirst;

		while (node->next != nullptr)
		{
			if (node->next->value == value)
			{
				node->next = node->next->next;
				count--;
				return;
			}
			node = node->next;
		}
		std::cout << "value was not found";
	}
};

// template <>
// class List<std::string>::applyToAll
// {
// public:
// 	void applyToAll(const std::function<void(std::string&)>& function)
// 	{
// 		if (count != 0)
// 		{
// 			Node<T>* node = mFirst;
		
// 			while (node != nullptr) 
// 			{
// 				function(node->value);
// 				node = node->next;
// 			}
// 		}
// 	}
// };


class X
{
public:
	X()
	{

	}

	void modifyElement(std::pair<std::string, std::size_t>& element)
	{
		element.second = element.first.length() + 1;
	}
};

template <typename T>
void foo(const List<T>& list)		// doesn't really work, i didn't find another way to print the element based on the type 
{
	list.applyToAll([](auto& element)
	{
		if (static_cast<std::pair<>>(element) != nullptr)
		{
			std::cout << element.second << ", ";
		}
		// if (static_cast<...>(element) != nullptr)	
		else 		//
		{
			std::cout << element << ", ";
		}
	});
}

template <typename T>
void foo2(const List<T>& list)
{
	list.applyToAll([](auto& element)
	{
		element.pop_back();
	});
}

// using PointerToMemberFunction = void (X::*)(std::pair<std::string, std::size_t>&);

int main()
{
	List<std::pair<std::string, std::size_t>> myList{};

	myList.insert(std::make_pair("dog", 0));
	myList.insert(std::make_pair("cpp-training-test", 0));

	myList.applyToAll([](auto& element)
	{
		element.second = element.first.length();
	});

	myList.applyToAll([](auto& element)
	{
		std::cout << element.second << ", ";
	});

	X x{};

	// PointerToMemberFunction ptr = &X::modifyElement;
	// auto ptr = &X::modifyElement;


	myList.applyToAll(std::bind(&X::modifyElement, x, std::placeholders::_1));

	myList.applyToAll([](auto& element)
	{
		std::cout << element.second << ", ";
	});

	List<std::string> myList2{};
	myList2.insert(std::string{"alex"});
	myList2.insert(std::string{"alex2"});

	// foo(myList); 
	foo(myList2);
	foo2(myList2);
	foo(myList2);

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
