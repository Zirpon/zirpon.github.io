//
//  VariadicTemplates.cpp
//  testdddd
//
//  Created by Cheung Zirpon on 2018/5/10.
//  Copyright © 2018年 Cheung Zirpon. All rights reserved.
//

#include "VariadicTemplates.hpp"

void printX(void){}

template<typename T, typename... Types>
void printX(const T& firstArg, const Types&... args)
{
    cout << firstArg << endl;
    printX(args...);
}

void hahaha()
{
    printX(19, "DDDD", 'd');
    printX(19, bitset<16>(8), "DDDD", 'd');
    cout << "dddd" <<endl;
}
