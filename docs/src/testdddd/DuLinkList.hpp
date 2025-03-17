//
//  DuLinkList.hpp
//  testdddd
//
//  Created by Cheung Zirpon on 2018/5/5.
//  Copyright © 2018年 Cheung Zirpon. All rights reserved.
//

#ifndef DuLinkList_hpp
#define DuLinkList_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>

struct node
{
    int data;
    node* pre;
    node* next;
};

bool DuLinkListInsert(node* *LinkListHead, int value);
void DuLinkListTravers(node* head);
bool DuLinkListDelete(node* *head, int value);
void CreateRandArray(int length);
void testDuLinkList();
#endif /* DuLinkList_hpp */
