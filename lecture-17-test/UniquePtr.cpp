#include <cstdarg>
#include <iostream>

template <typename T>
class UniquePtr
{
public:

	UniquePtr()
	: mPointedValue{new T{}}
	{
		testCount++;
	}

	UniquePtr(const T& value)
	: mPointedValue{new T{value}}
	{
		testCount++;
	}

	UniquePtr(T* ptr)
	: mPointedValue{ptr}
	{
		testCount++;
	}

	UniquePtr(UniquePtr<T>&& a)
	: mPointedValue{a.mPointedValue}
	{
		testCount++;
		a.mPointedValue = nullptr;
	}

	static UniquePtr<T> Move(UniquePtr<T>& a)		// Move se comporta ca std::move
	{
		UniquePtr<T> temp{a.mPointedValue};
		a.mPointedValue = nullptr;
		return temp;
	}

	template <typename... Ts>
	static UniquePtr<T> Make(Ts... args)			// Make e echivalentul lui std::make_unique
	{
		return UniquePtr<T>(new T{args...});
	}


	T& release()
	{
		T& ret = *mPointedValue;
		mPointedValue = nullptr;
		return ret;
	}

	UniquePtr(const UniquePtr& ptr) = delete;

	T* getRawPointer()
	{
		return mPointedValue;
	}

	T& operator*()
	{
		return *mPointedValue;
	}

	const T& operator*() const
	{
		return *mPointedValue;
	}

	T* operator->() const
	{
		return mPointedValue;
	}

	bool operator==(UniquePtr<T> a) const
	{
		return mPointedValue == a.mPointedValue;
	}

	bool operator!=(UniquePtr<T> a) const
	{
		return mPointedValue != a.mPointedValue;
	}

	bool operator!() const
	{
		return mPointedValue == nullptr;
	}

	~UniquePtr()
	{
		testCount--;
		delete mPointedValue;
		mPointedValue = nullptr;
	}

	static int getCount()
	{
		return testCount;
	}

	UniquePtr<T> operator=(UniquePtr<T>& a)
	{
		UniquePtr<T> temp = UniquePtr<T>::Move(a);
		return temp;
	}

private:
	T* mPointedValue;

	static int testCount;
};

class X
{
public:
	X()
	: member1{},
	  member2{},
	  member3{}
	{

	}

	X(int a, int b, std::string c)
	: member1{a},
	  member2{b},
	  member3{c}
	{

	}  

	void foo()
	{
		member1 = 10;
	}

	void print()
	{
		std::cout << member1 << ", " << member2 << ", " << member3 << std::endl;
	}

private:
	int member1;
	int member2;
	std::string member3;
};


class C
{
public:
	C(int value)
	: member{value}
	{

	}

	void printMember()
	{
		std::cout << member << "\n";
	}

	int getMember()
	{
		return member;
	}

private:
	int member;
};

template <typename T>
class Test
{
public:
	Test()
	: mValue{}
	{

	}

	Test(UniquePtr<T> value)
	: mValue{value}
	{

	}

	T getValue()
	{
		return *mValue;
	}


private:
	UniquePtr<T> mValue{};
};

template <typename T>
int UniquePtr<T>::testCount = 0;		// initializare globala a testCount

void testUniquePtr_Move()
{
	int p = 10;
	int initialCount = UniquePtr<int>::getCount();
	UniquePtr<int> ptr{p};
	UniquePtr<int> ptr2 = UniquePtr<int>::Move(ptr);
	int count = UniquePtr<int>::getCount() - initialCount;

	if (count == 2)
	{
		std::cout << "move test passed\n";
	}
	else 
	{
		std::cout << "move test failed, " << count << " instances created\n";
	}
}

void testUniquePtr_Make()
{
	int initialCount = UniquePtr<X>::getCount();
	UniquePtr<X> ptrToX = UniquePtr<X>::Make(1, 2, "name");
	int count = UniquePtr<X>::getCount() - initialCount;

	ptrToX->print();

	if (count == 1)
	{
		std::cout << "make test passed\n";
	}
	else 
	{
		std::cout << "make test failed, " << count << " instances created\n";
	}
}

int main()
{
	// a = 11;
	// Test t{a};
	// std::cout << t.getValue() << "\n";
	

	// UniquePtr
	//
	std::cout << "UniquePtr \n";
	int a = 10;
	UniquePtr<int> ptrA{a};
	std::cout << *ptrA << "\n";

	UniquePtr<C> ptrC{20};
	ptrC->printMember();

	auto ptrB = UniquePtr<int>::Move(ptrA);
	// auto ptrB = ptrA;
	std::cout << *ptrB << "\n";


	UniquePtr<int> temp{15};

	// (1)
	// ptrA = temp;
	
	// (2)			
	//
	auto ptrAux = UniquePtr<int>::Make(15);
	std::cout << *ptrAux << "\n";

	// (3)
	ptrAux.release();

	// (4)
	assert (ptrAux == nullptr);

	// (5)
	assert (!ptrAux);

	UniquePtr<int> ptrA5{nullptr};


	// unique_ptr
	//
	std::cout << "\n\nstd::unique_ptr\n";
	std::unique_ptr<int> ptrA1 = std::make_unique<int>(10);
	std::cout << *ptrA1 << "\n";

	std::unique_ptr<C> ptrC1 = std::make_unique<C>(20);
	ptrC1->printMember();

	auto ptrB1 = std::move(ptrA1);
	std::cout << *ptrB1 << "\n";


	std::unique_ptr<int> temp1 = std::make_unique<int>(15);

	// (1): nu pot face assignarea de mai jos
	// ptrA1 = ptrA2;

	// (2): trebuie sa mearga
	ptrA1 = std::move(temp1);

	// (3): release
	ptrA1 = nullptr;

	//(4): sa fie bool
	assert(ptrA1 == nullptr);

	//(5): sa functioneze mai jos
	assert(!ptrA1);


	//(6):
	std::unique_ptr<int> ptrA3{};
	std::unique_ptr<int> ptrA4{nullptr};



	testUniquePtr_Move();
	testUniquePtr_Make();
	// + UNIT TEST Make construieste o singura instanta de T
	// sa pice in cazul in care se apeleaza de mai multe ori constructorul lui T (sau celelalte, ex copy)

	return 0;

	// Questions:
	//
	// 1. Write a template class UniquePtr that behaves as much as a std::unique_ptr as possible. Do not try to be perfect, but support the basics at least.
	//
	// 2. Write a class Test that takes and saves as member a UniquePtr<int>.
}
