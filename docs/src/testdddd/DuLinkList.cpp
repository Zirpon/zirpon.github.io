//
//  DuLinkList.cpp
//  testdddd
//
//  Created by Cheung Zirpon on 2018/5/5.
//  Copyright © 2018年 Cheung Zirpon. All rights reserved.
//

#include "DuLinkList.hpp"
using namespace std;

bool DuLinkListInsert(node* *head, int value)
{
    if (head == NULL) {
        return false;
    }
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL) {
        return false;
    }
    newNode->data = value;
    newNode->pre = newNode->next = newNode;
    
    if (*head == NULL) {
        *head = newNode;
        return true;
    }

    node* linkHead = *head;
    if (linkHead->pre == linkHead && linkHead->next == linkHead) {
        linkHead->next = newNode;
        linkHead->pre = newNode;
        newNode->pre = linkHead;
        newNode->next = linkHead;
        if (linkHead->data > value) {
            *head = newNode;
        }
        return true;
    }
    
    node* pNode = linkHead;
    while (pNode && (pNode->next != linkHead) && pNode->data < value) {
        pNode = pNode->next;
    }
    
    if (pNode->data > value) {
        newNode->pre = pNode->pre;
        newNode->next = pNode;
        pNode->pre->next = newNode;
        pNode->pre = newNode;
        
        if (linkHead == pNode) {
            *head = newNode;
        }

    }
    else {
        newNode->pre = pNode;
        newNode->next = pNode->next;
        pNode->next->pre = newNode;
        pNode->next = newNode;
    }
    
    return true;
}

void DuLinkListTravers(node* head)
{
    if (head != NULL) {
        node* pNode = head;
        stringstream outStr;
        while (pNode && pNode->next != head) {
            outStr << pNode->data << ',';
            pNode = pNode->next;
        }
        outStr << pNode->data << ',';

        cout << outStr.str() << endl;
    }
}

bool DuLinkListDelete(node* *head, int value)
{
    if (head == NULL || *head == NULL) {
        return false;
    }
    
    node* linkHead = *head;
    if (linkHead->pre == linkHead && linkHead->next == linkHead) {
        free(linkHead);
        *head = NULL;
        return true;
    }
    
    node* pNode = linkHead;
    bool flag = false;
    while (pNode && pNode->next != linkHead) {
        if (pNode->data == value) {
            flag = true;
            break;
        }
        pNode = pNode->next;
    }
    
    if (flag) {
        pNode->pre->next = pNode->next;
        pNode->next->pre = pNode->pre;
        
        if (pNode == linkHead) {
            *head = pNode->next;
        }
        
        free(pNode);
        return true;
    }
    
    return false;
}

void CreateRandArray(int length)
{
    int * array = (int*)malloc(length*sizeof(int));
    //int array[length] = {0};
    std::stringstream outStr;
    node* LinkHead = NULL;
    for (int i = 0; i < length; i++) {
        array[i] = rand()%100;
        outStr << array[i] << ',';
        DuLinkListInsert(&LinkHead, array[i]);
        //DuLinkListTravers(LinkHead);
    }
    cout << outStr.str() << endl;
    DuLinkListTravers(LinkHead);
    for (int i = 0; i < length; i++) {
        DuLinkListDelete(&LinkHead, array[i]);
        DuLinkListTravers(LinkHead);
    }
    free(array);
}

void testDuLinkList()
{
    int array[] = {3,4,5,6,3};
    std::stringstream outStr;
    node* LinkHead = NULL;
    for (int i = 0; i < 5; i++) {
        outStr << array[i] << ',';
        DuLinkListInsert(&LinkHead, array[i]);
        //DuLinkListTravers(LinkHead);
    }
    cout << outStr.str() << endl;
}






