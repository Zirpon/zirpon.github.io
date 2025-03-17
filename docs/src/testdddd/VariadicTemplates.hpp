//
//  VariadicTemplates.hpp
//  testdddd
//
//  Created by Cheung Zirpon on 2018/5/10.
//  Copyright © 2018年 Cheung Zirpon. All rights reserved.
//

#ifndef VariadicTemplates_hpp
#define VariadicTemplates_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

void hahaha();

void printX(void);

template<typename T, typename... Types>
void printX(const T& firstArg, const Types&... args);



#endif /* VariadicTemplates_hpp */
