//
//  main.cpp
//  574project2
//
//  Created by mdong3 on 11/13/15.
//  Copyright (c) 2015 tianqing. All rights reserved.
//

//#include <openssl/conf.h>
//#include <openssl/evp.h>
//#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <string.h>
using namespace std;
int main(int arc, char *argv[])
{
   ////////////////////////////////////////////////
//    unsigned char key[16];//, iv[16];
//    
//    if (!RAND_bytes(key, sizeof key)) {
//        /* OpenSSL reports a failure, act accordingly */
//        cout << "fail" << endl;
//    }
//    cout << "code: " << endl;
//    for (int i = 0; i < 16; i++) {
//        cout << key[i] << endl;
//    }
  ////////////////////////////////////////////////
    FILE* sendFile;
    sendFile = fopen("sendFile.txt", "w+");
    ////////////////Generate 32 characters string as the session key/////////////////
    int secIndx;
    string sessionPsw;

    /* initialize random seed: */
    srand( static_cast<unsigned int>(time(NULL)));
    for (int i = 0; i < 32; i ++) {

        /* generate secret number between 0 and 61: */
        secIndx = rand() % 62;
//        cout << i << ": " << secIndx << endl;
        if (secIndx <= 9) {
            char c = '0' + secIndx;
            sessionPsw += c;
        }
        else if (secIndx >= 10 && secIndx <= 35) {
            char c = 'A' + secIndx - 10;
            sessionPsw += c;
        }
        else {
            char c = 'a' + secIndx - 36;
            sessionPsw += c;
        }
    }
//    cout << sessionPsw << endl;
    /////////////////////////////////////////////////////////////
    ////////////////////input the message text///////////////////
    int messageC;
    FILE *messageFile;
    messageFile = fopen ("message.txt","w+");
    puts ("Enter text. Include a star ('*') in a sentence to exit:");
    while(1) {
        messageC = getchar();
//        putchar (messageC);
        if (messageC == '*')
            break;
        fprintf(messageFile, "%c", messageC);
    }
    fclose(messageFile);
    ////////////////////////////////////////////////////////////
    /////////////////////encrypt the message////////////////////
    string encryptShell = "./encryptMes.sh ";
    encryptShell += sessionPsw;
    cout << "start to encrypt message!" << endl;
    system(encryptShell.c_str());
    cout << "done!" << endl;
    ////////////////////////////////////////////////////////////

    fclose(sendFile);
    return 0;
}
