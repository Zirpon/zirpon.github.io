//
//  SingleLink.cpp
//  testdddd
//
//  Created by Cheung Zirpon on 2018/5/6.
//  Copyright © 2018年 Cheung Zirpon. All rights reserved.
//

#include "SingleLink.hpp"
using namespace std;

bool SingleLinkInsert(node* *head, int value)
{
    if (head == NULL) {
        return false;
    }
    
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL) {
        return false;
    }
    newNode->data = value;
    newNode->next = NULL;
    node* pNode = *head;
    if (pNode == NULL) {
        *head = newNode;
        return true;
    }
    while (pNode && pNode->next != NULL) {
        pNode = pNode->next;
    }
    
    pNode->next = newNode;
    return true;
}

void SingleLinkTraverse(node* head)
{
    stringstream outStr;
    node* pNode = head;
    while (pNode) {
        outStr << pNode->data << ",";
        pNode = pNode->next;
    }
    cout << outStr.str() << endl;
}

bool SingleLinkReverse(node* *head)
{
    if (head == NULL || *head == NULL) {
        return false;
    }
    
    node* pre = *head;
    node* cur = *head;
    node* linkHead = *head;
    
    while (cur && cur->next) {
        node * tmp = cur->next->next;
        node* tmp2 = cur->next;
        cur->next->next = cur;
        if (cur == linkHead) {
            cur->next = NULL;
        }
        else
        {
            cur->next = pre;
        }
        pre = tmp2;
        cur = tmp;
    }
    
    if (cur == NULL) {
        *head = pre;
    }
    else
    {
        if (pre != NULL) {
            cur->next = pre;
        }
        *head = cur;
    }
    return true;
}

void CreateSinglelinkRandArray(int length)
{
    int * array = (int*)malloc(length*sizeof(int));
    std::stringstream outStr;
    node* LinkHead = NULL;
    for (int i = 0; i < length; i++) {
        array[i] = rand()%100;
        outStr << array[i] << ',';
        SingleLinkInsert(&LinkHead, array[i]);
        //SingleLinkTraverse(LinkHead);
    }
    cout << "originalArray:\n" << outStr.str() << endl;
    SingleLinkTraverse(LinkHead);
    SingleLinkReverse(&LinkHead);
    SingleLinkTraverse(LinkHead);
    outStr.str("");
    for (int i = length - 1; i >= 0; i--) {
        outStr << array[i] << ',';
    }
    cout << "reverseArray:\n" << outStr.str() << endl;

    free(array);
}

void testSingleLinkList()
{
    int array[] = {3,4,5,6,3};
    std::stringstream outStr;
    node* LinkHead = NULL;
    for (int i = 0; i < 5; i++) {
        outStr << array[i] << ',';
        SingleLinkInsert(&LinkHead, array[i]);
        //SingleLinkTraverse(LinkHead);
    }
    cout << outStr.str() << endl;
}









