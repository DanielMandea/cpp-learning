#include <iostream> 

class A
{ 
public: 
    ~A()
    {
        std::cout << "~A() \n\n";
    } 

    virtual void f1(const int &a = 0)
    {  
        std::cout << "A::f1() value = " << a << std::endl; 
    } 

    virtual void f2()
    { 
        std::cout << "A::f2()" << std::endl; 
    } 

    void f3()
    { 
        std::cout << "A::f3()" << std::endl;  
    }
}; 

class B : public A
{ 
public:
    ~B()
    {
        std::cout << "~B() \n\n";
    } 

    void f1(const int &a = 1)
    { 
        std::cout << "B::f1() value = " << a << std::endl; 
    } 

    void f2()
    { 
        std::cout << "B::f2()" << std::endl; 
    } 

    void f3()
    {
        std::cout << "B::f3()" << std::endl; 
    } 
}; 

int main()
{
    A *inst0 = new B[*new int{10}];
    inst0[0].f1();
    inst0[0].f2();
    inst0[0].f3();

    A *inst1 = new B(); 
    inst1->f1(); 
    inst1->f2(); 
    inst1->f3(); 

    B *inst2 = new B(); 
    inst2->f1(500); 
    inst2->f2(); 
    inst2->f3(); 

    A *inst3 = new A(); 
    inst3->f1(5); 
    inst3->f2(); 
    inst3->f3();

    std::cout << "\n\n\n";

    delete inst0;
    delete inst1; 
    delete inst2; 
    delete inst3; 

    return 0;

    // Questions:
    //
    // 1. What is the output of the program?
    //
    // 2. What potential issues can you find with this implementation? Propose any kind of changes that would improve it.
}
