//
//  ByteArray.hpp
//  testdddd
//
//  Created by Cheung Zirpon on 2018/5/12.
//  Copyright © 2018年 Cheung Zirpon. All rights reserved.
//

#ifndef ByteArray_hpp
#define ByteArray_hpp

#include <stdio.h>
#include <iostream>
#include <cstring>
using namespace std;

struct Packet
{
    int a;
    char c;
    float l;
    int b;
    char str[32];
};


void conv();
#endif /* ByteArray_hpp */
