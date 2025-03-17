//
//  UserDefineLiterals.cpp
//  testdddd
//
//  Created by Cheung Zirpon on 2018/5/12.
//  Copyright © 2018年 Cheung Zirpon. All rights reserved.
//

#include "UserDefineLiterals.hpp"

int operator"" _bin(const char* std, size_t n)
{
    int ret = 0;
    for (int i = 0; i < n; i++) {
        ret = ret << 1;
        if (std[i] == '1') {
            ret += 1;
        }
    }
    
    cout << "n=" << n << endl;
    return ret;
}

void testLiterals()
{
    long double a = 9.0_cm;
    cout << a << endl;
    char k = 'd';
    char16_t p = 's';
    char32_t d = 's';
    wchar_t m = 's';
    cout << sizeof(p) << endl;
    cout << sizeof(d) << endl;
    cout << sizeof(k) << endl;
    cout << sizeof(m) << endl;
    cout << "110"_bin << endl;
}
