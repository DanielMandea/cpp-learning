#include <cstddef>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

template <typename T>
struct Node {
	Node<T>* next;
	T data;
};

template <typename T>
class LinkedList
{
public:
	LinkedList<T>() 
	:mFirst{nullptr},
	 mLast{nullptr}
	{
		
	}

	void insert(T data) {
		if(!mFirst) 
		{
			// list is empty
			mFirst = new Node<T>;
			mFirst->data = data;
			mFirst->next = nullptr;
			mLast = mFirst;
		} 
		else 
		{
			// list has one element
			if(mLast == mFirst)
			{	
				mLast = new Node<T>;
				mLast->data = data;
				mLast->next = nullptr; // pointer to the next element will be null because it has no address to point to.
				mFirst->next = mLast;
			} 
			else 
			{
				// list has more than one element
				Node<T>* insdata = new Node<T>;
				insdata->data = data;
				insdata->next = nullptr;
				mLast->next = insdata;
				mLast = insdata;
			}
		}
	}

	//implement applyToAll + line 41 bug 
	
	void applyToAll(const std::function<void(std::pair<std::string, std::size_t>&)>& function)
	{
		if (mFirst != mLast)
		{
			Node<T>* node = mFirst;

			do 
			{
				function(node->data);
				node = node->next;

			}while (node != nullptr);
		}
	}

private:
	Node<T>* mFirst;
	Node<T>* mLast;
};

class Helper
{
public:
	Helper()
	:mHelper{}
	{

	}

	void apply(std::pair<std::string, std::size_t>& element)
	{
		element.second = element.first.length();
	}
	
	void printList(std::pair<std::string, std::size_t>& element)
	{
	    std::cout << element.second << ", ";
	}

private:
	LinkedList<std::pair<std::string, std::size_t>> mHelper;
};



int main()
{
	Helper myHelper{};
	LinkedList<std::pair<std::string, std::size_t>> myList{};

	myList.insert(std::make_pair("dog", 0));
	myList.insert(std::make_pair("cpp-training-test", 0));
	
	std::pair<std::string, std::size_t> el1 = std::make_pair("dog", 0);
	std::pair<std::string, std::size_t> el2 = std::make_pair("cpp-training-test", 0);

// 	myList.applyToAll([](auto& element)
// 	{
// 		element.second = element.first.length();
// 	});

// 	myList.applyToAll([](auto& element)
// 	{
// 		std::cout << element.second << ", ";
// 	});

	myList.applyToAll(std::bind(&Helper::apply, myHelper, std::placeholders::_1));
	myList.applyToAll(std::bind(&Helper::printList, myHelper, std::placeholders::_1));

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