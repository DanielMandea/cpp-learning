#include <iostream>


class WidgetIntf
{
public:
	virtual void display() const = 0;

	virtual void prepareToDisplay() = 0;
};



class Widget : public WidgetIntf
{
public:
	Widget()
	{

	}

	void display() const override
	{
		
	}

	void prepareToDisplay() override
	{

	}
};


class AnotherWidget : public WidgetIntf
{
public:
	AnotherWidget()
	{

	}

	void display() const override
	{
		
	}

	void prepareToDisplay() override
	{
		
	}
};


void displayWidget(WidgetIntf& w)
{
	// in case there exists a "WidgetIntf" interface from which both "Widget" and "AnotherWidget" are derived, then we can use polymorphism and 
	// write our "displayWidget" function in a polymorphic way, i.e. take a "WidgetIntf" reference as parameter and let the runtime call the appropriate method depending on the specific type
	//

	w.prepareToDisplay();

	w.display();
}


template <typename WidgetT>
void displayWidget(WidgetT& widget)
{
	// but what if "Widget" and "AnotherWidget" are completely separate classes, e.g. they do not share a common interface.
	//
	// yet somewhere in the code I need to make a sequence of identical calls on two objects of these two unrelated classes (Widget and AnotherWidget)
	//
	// in this case, I use a template function that takes an arbitrary T as a template parameter, and then its actual function parameter is a reference of type T
	//
	// Requirements and Contraints can be applied to the arbitrary "T" type with the use of "Concepts" -- a C++20-introduced mechanism
	// e.g. maybe I want my T type to be "Hashable" or "Default-Constructible", etc
	//
	widget.prepareToDisplay();

	widget.display();
}

// just like template functions, this is a template class
template <typename T>
class Test
{
public:
	Test()
	{

	}

	// and what is this? This is a template method! Or "template member function", as the slang in C++ goes.
	//
	// this is unrelated to the fact that the containing class "Test" happens to be a template class.
	//
	// we could have a template member function in a non-template class, or the other way around.
	//
	// notice how it can use both the class template parameter T and its own template parameter U
	//
	template <typename U>
	void doSomething(const T& t, const U& u)
	{

	}

private:
	T t;
};


int main()
{
	Widget w{};
	AnotherWidget anotherW{};

	displayWidget<Widget>(w);
	displayWidget<AnotherWidget>(anotherW);

	// what if I tried to call displayWidget<Widget>(anotherWidget) ? and why?


	// this "t" variable is an instance of Test<int>.
	//
	// what is Test<int> ? It is a SPECIALIZATION (C++ slang) of the template class Test,
	// meaning no longer a template but a concrete class where we already know all template parameters, in this case they are T == int !
	//
	Test<int> t{};

	// now what is this? This is using the "t" variable, which is an instance of Test<int>, to call a template member function !
	// 
	// see how the T is int and the U is double?
	//
	t.doSomething<double>(10, 5.5);

	return 0;
}
