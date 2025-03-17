//
//  main.cpp
//  testdddd
//
//  Created by Cheung Zirpon on 2018/5/5.
//  Copyright © 2018年 Cheung Zirpon. All rights reserved.
//

#include "main.hpp"
//#include "DuLinkList.hpp"
//#include "SingleLink.hpp"
//#include "StringUtils.hpp"
//#include "VariadicTemplates.hpp"

using namespace std;

extern void CreateRandArray(int length);
extern void testDuLinkList();
extern void CreateSinglelinkRandArray(int length);
extern void testSingleLinkList();
extern void testStringUtils();

extern void hahaha();
extern void testLiterals();
extern void conv();

//template<typename T, typename... Types>
//extern void printX(const T& firstArg, const Types&... args);

class A{
public:
    A(){cout << "A constructor" << endl;}
    A(long value) : m_iData(value) {}
public:
    A(initializer_list<int> initList)
    {
        for (auto i : initList) {
            cout << i << endl;
        }
    }
    void inline test_2(){for (long i = 1; i > 0; i++) m_iData++;cout << m_iData << endl;};
    void count(){cout << "a" << endl;}
public:
    long m_iData = 0;
    //~A() = delete;
    void test_A() = delete;
public:
    static auto test_A_2(int x, float y) -> decltype(x+y)
    {
        return x+y;
    }
};

class B : public A
{
    void count(){cout << "b" << endl;}
}
;

void testA_smartPointer(A* ptr)
{
    cout << ptr->m_iData << endl;
}

int main(int argc, const char * argv[]) {
    
    //testDuLinkList();
    //CreateRandArray(50);
    //CreateSinglelinkRandArray(3);
    testStringUtils();
    std::cout << "Hello, World!\n";
    
    //----------------------C++ 11 test ----------------------------------------
    A *b = new A;
    cout << b->test_A_2(3, 5.9) << endl;
    auto p = &A::test_A_2;
    cout << p(4,56.7) << endl;
    //b->test_2();
    int bb = 0;
    int cc = 1;
    
    auto i = [&bb, &cc](){
        int cc = bb+1;
        bb++;
        cc++;
        cout << cc << " " << bb << endl;
    };
    bb = 99;
    i();
    i();
    cout << cc << endl;
    //A a{1,2,3,4,56,67,8,89};

    hahaha();
    
    //printX(19, "DDDD", 'd');
    //printX(19, bitset<16>(8), "DDDD", 'd');
    testLiterals();
    
    //----------------------C++ 11 test shared_pointer ----------------------------------------
    cout << "//----------------------C++ 11 test shared_pointer ----------------------------------------" << endl;
    shared_ptr<A> Aptr = make_shared<A>(100);
    A * rawApointer = Aptr.get();
    testA_smartPointer(rawApointer);
    cout << rawApointer->m_iData << endl;
    cout << Aptr->m_iData << endl;
    
    cout << "//----------------------C++ 11 test ByteArray ----------------------------------------" << endl;
    conv();
    
    A* testcount = new B;
    testcount->count();
    
    return 0;
}
