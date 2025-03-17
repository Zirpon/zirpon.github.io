//
//  main.cpp
//  testdddd
//
//  Created by Cheung Zirpon on 2018/4/12.
//  Copyright © 2018年 Cheung Zirpon. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <math.h>

int getResult(int num) {
    if (num < 10) return num;
    int temp = num;
    int sum = 0;
    while (temp != 0) {
        sum += temp % 10;
        temp /= 10;
    }
    return getResult(sum);
}
//-----------------------------------------链表闭环-----------------------------------
typedef struct Node
{
    int val;
    Node *next;
}Node,*pNode;

//判断是否有环
bool isLoop(pNode pHead)
{
    pNode fast = pHead;
    pNode slow = pHead;
    //如果无环，则fast先走到终点
    //当链表长度为奇数时，fast->Next为空
    //当链表长度为偶数时，fast为空
    while( fast != NULL && fast->next != NULL)
    {
        
        fast = fast->next->next;
        slow = slow->next;
        //如果有环，则fast会超过slow一圈
        if(fast == slow)
        {
            break;
        }
    }
    
    if(fast == NULL || fast->next == NULL  )
        return false;
    else
        return true;
}

//计算环的长度
int loopLength(pNode pHead)
{
    if(isLoop(pHead) == false)
        return 0;
    pNode fast = pHead;
    pNode slow = pHead;
    int length = 0;
    bool begin = false;
    bool agian = false;
    while( fast != NULL && fast->next != NULL)
    {
        fast = fast->next->next;
        slow = slow->next;
        //超两圈后停止计数，挑出循环
        if(fast == slow && agian == true)
            break;
        //超一圈后开始计数
        if(fast == slow && agian == false)
        {
            begin = true;
            agian = true;
        }
        
        //计数
        if(begin == true)
            ++length;
        
    }
    return length;
}


//求出环的入口点
Node* findLoopEntrance(pNode pHead)
{
    pNode fast = pHead;
    pNode slow = pHead;
    while( fast != NULL && fast->next != NULL)
    {
        
        fast = fast->next->next;
        slow = slow->next;
        //如果有环，则fast会超过slow一圈
        if(fast == slow)
        {
            break;
        }
    }
    if(fast == NULL || fast->next == NULL)
        return NULL;
    slow = pHead;
    while(slow != fast)
    {
        slow = slow->next;
        fast = fast->next;
    }
    
    return slow;
}
//-----------------------------------------大小端-----------------------------------

bool isBigEndian_2()
{
    union uendian
    {
        int nNum;
        char cLowAddressValue;
    };
    
    uendian u;
    u.nNum = 0x123456;
    
    if ( u.cLowAddressValue == 0x12 )     return true;
    
    return false;
}

// @Ret: 大端，返回true; 小端，返回false
bool IsBigEndian_1()
{
    int nNum = 0x12345678;
    char cLowAddressValue = *(char*)&nNum;
    
    // 低地址处是高字节，则为大端
    if ( cLowAddressValue == 0x12 )    return true;
    if (cLowAddressValue == 0x78) {
        return false;
    }
    
    return false;
}

//-----------------------------------------快速排序-----------------------------------
template<typename T>
void qsort(T lst[], int head, int tail) {
    /*
     * 功能：对数组升序快排，递归实现
     * 参数：lst：带排序数组，head：数组首元素下标，tail：数组末元素下标
     * 返回值：无
     */
    
    if (head >= tail) return ;
    
    int i = head, j = tail;
    
    T pivot = lst[head];  // 通常取第一个数为基准
    
    while (i < j) { // i,j 相遇即退出循环
        while (i < j and lst[j] >= pivot) j--;
        lst[i] = lst[j];    // 从右向左扫描，将比基准小的数填到左边
        while (i < j and lst[i] <= pivot) i++;
        lst[j] = lst[i];    //  从左向右扫描，将比基准大的数填到右边
    }
    
    lst[i] = pivot; // 将 基准数 填回
    
    qsort(lst, head, i - 1);    // 以基准数为界左右分治
    qsort(lst, j + 1, tail);
}

//-----------------------------------------归并排序-----------------------------------
void Merge(int sourceArr[],int tempArr[], int startIndex, int midIndex, int endIndex)
{
    int i = startIndex, j=midIndex+1, k = endIndex;
    while(i!=midIndex+1 && j!=endIndex+1)
    {
        if(sourceArr[i] > sourceArr[j])
            tempArr[k++] = sourceArr[j++];
        else
            tempArr[k++] = sourceArr[i++];
    }
    while(i != midIndex+1)
        tempArr[k++] = sourceArr[i++];
    while(j != endIndex+1)
        tempArr[k++] = sourceArr[j++];
    for(i=startIndex; i<=endIndex; i++)
        sourceArr[i] = tempArr[i];
}

//内部使用递归
void MergeSort(int sourceArr[], int tempArr[], int startIndex, int endIndex)
{
    int midIndex;
    if(startIndex < endIndex)
    {
        midIndex = (startIndex + endIndex) / 2;
        MergeSort(sourceArr, tempArr, startIndex, midIndex);
        MergeSort(sourceArr, tempArr, midIndex+1, endIndex);
        Merge(sourceArr, tempArr, startIndex, midIndex, endIndex);
    }
}
/*
int main(int argc, char * argv[])
{
    int a[8] = {50, 10, 20, 30, 70, 40, 80, 60};
    int i, b[8];
    MergeSort(a, b, 0, 7);
    for(i=0; i<8; i++)
        printf("%d ", a[i]);
    printf("\n");
    return 0;
}
*/
int main(int argc, const char * argv[]) {
    
    //-----------------------------------------各位相加-----------------------------------
    long long aa = 0x7fffffffe;
    //int arr[0x7fffffffe];
    
    bool ret = IsBigEndian_1();
    printf("ret=%d", ret);
    // insert code here...
    //-----------------------------------------各位相加-----------------------------------
    int b = 81;
    int a = getResult(b);
    printf("a=%d,b=%d", a, b%9);
    //-----------------------------------------十进制转换七进制-----------------------------------
    int num = 100;
    int mod = 0;
    int shang = 0;
    shang = num / 7;
    mod = num % 7;
    if (0 == shang)
    {
        printf("result=%d", mod);
    }
    else
    {
        int result = 0;
        int times = 0;
        while(true)
        {
            int test = pow(10, times);
            int temp = mod*test;
            result += temp;
            times++;
            if (0 == shang) {
                break;
            }
            int tmp = shang;
            shang = tmp / 7;
            mod = tmp % 7;
        }
        printf("result=%d",result);
    }
    //sleep(100);

    std::cout << "Hello, World!\n";
    return 0;
}
