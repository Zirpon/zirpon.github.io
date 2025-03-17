//
//  StringUtils.cpp
//  testdddd
//
//  Created by Cheung Zirpon on 2018/5/6.
//  Copyright © 2018年 Cheung Zirpon. All rights reserved.
//

#include "StringUtils.hpp"
using namespace std;

const char* mergeSpace(const char* src, char* dest, unsigned long destSize)
{
    if (src == NULL || dest == NULL) {
        return NULL;
    }
    
    unsigned long srcSize = strlen(src) + 1;
    destSize = srcSize < destSize ? srcSize : destSize;
    const char* cur = src;
    const char* pre = src;
    char* dptr = dest;
    char* dEnd = dest + destSize;
    while (*cur != '0' && dptr < dEnd) {
        if (pre == cur) {
            *dptr++ = *cur++;
            continue;
        }
        else if (*pre == '_' && *cur == '_')
        {
            cur++;
            continue;
        }
        else if ((*pre != *cur) || (*pre == *cur && *pre != '_'))
        {
            *dptr++ = *cur;
            pre = cur++;
            continue;
        }
    }
    
    *dptr = '0';

    return dest;
}

void testStringUtils()
{
    char src[] = {"__H___lll_aaa_______________________"};
    char dest[100] = {0};
    stringstream outStr;
    
    mergeSpace(src, dest, sizeof(dest));
    outStr << dest;
    cout << outStr.str() << endl;
}




