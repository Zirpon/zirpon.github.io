//
//  ByteArray.cpp
//  testdddd
//
//  Created by Cheung Zirpon on 2018/5/12.
//  Copyright © 2018年 Cheung Zirpon. All rights reserved.
//

#include "ByteArray.hpp"


void conv()
{
    Packet tmpa{99,'M',9.0,100,"hello world"};
    char buf[100] = {0};
    char* pBuf = buf;
    memcpy((void*)pBuf, &tmpa, sizeof(tmpa));
    
    for (int i = 0; i < 100; i++) {
        cout << pBuf[i] << ',';
    }
    
    cout << endl;

    Packet tmpb;
    memcpy(&tmpb, pBuf, sizeof(tmpb));
    cout << tmpb.a << endl;
    cout << tmpb.b << endl;
    cout << tmpb.c << endl;
    cout << tmpb.l << endl;
    cout << tmpb.str << endl;
    cout << "ddddddddddddddddd 2" << endl;

    int inta = *((int*)pBuf);
    char* pppp = pBuf+8;
    float fl = *((float*)pppp);
    cout << inta << endl;
    cout << fl << endl;
//    char * strp = pBuf+8;
  //  cout << strp << endl;
    
    cout << "ddddddddddddddddd" << endl;
    char buf2[100] = {0};
    char* buf2p = buf2;
    Packet* pkptr = (Packet*)buf2p;
    pkptr->a = 23;
    pkptr->b = 78;
    cout << sizeof(float) << endl;
    cout << sizeof(Packet) << endl;
    memcpy(pkptr->str, "jjjooo", sizeof(pkptr->str));
    cout << pkptr->a << endl;
    cout << pkptr->b << endl;
    cout << pkptr->str << endl;
    
    cout << "ddddddddddddddddd" << endl;

    for (int i = 0; i < 100 ; i++) {
        cout << buf2[i] << ",";
    }
    cout << "ddddddddddddddddd" << endl;

    cout << endl;
    
    
    
}
