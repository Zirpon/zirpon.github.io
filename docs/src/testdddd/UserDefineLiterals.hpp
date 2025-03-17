//
//  UserDefineLiterals.hpp
//  testdddd
//
//  Created by Cheung Zirpon on 2018/5/12.
//  Copyright © 2018年 Cheung Zirpon. All rights reserved.
//

#ifndef UserDefineLiterals_hpp
#define UserDefineLiterals_hpp

#include <stdio.h>
#include <iostream>
using namespace std;
constexpr long double operator"" _cm(long double x) {return x*10;}
constexpr long double operator"" _m(long double x) {return x*1000;}
constexpr long double operator"" _mm(long double x) {return x;}
int operator"" _bin(const char* std, size_t n);
void testLiterals();

#endif /* UserDefineLiterals_hpp */
