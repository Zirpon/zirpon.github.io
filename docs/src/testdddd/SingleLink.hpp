//
//  SingleLink.hpp
//  testdddd
//
//  Created by Cheung Zirpon on 2018/5/6.
//  Copyright © 2018年 Cheung Zirpon. All rights reserved.
//

#ifndef SingleLink_hpp
#define SingleLink_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

struct node
{
    int data;
    node* next;
};


bool SingleLinkInsert(node* *head, int value);
void SingleLinkTraverse(node* head);
bool SingleLinkReverse(node* *head);
void CreateSinglelinkRandArray(int length);
void testSingleLinkList();


#endif /* SingleLink_hpp */
