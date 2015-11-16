//
//  main.cpp
//  574project2
//
//  Created by mdong3 on 11/13/15.
//  Copyright (c) 2015 mdong3. All rights reserved.
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

void sendEmail();
void receiveEmail();

int main(int arc, char *argv[])
{
    cout << "Hi, enter 0 to send a secure mail, or enter 1 to reveive a secure mail." << endl;
    int choice;
//    cin >> choice;
    choice = 1;
    switch (choice) {
        case 0:
            sendEmail();
            break;
        case 1:
            receiveEmail();
        default:
            break;
    }
    return 0;
}

void sendEmail(){
    FILE* sendFile;
    cout << "please input the email name: ";
    string emailName;
    sendFile = fopen(emailName.c_str(), "w+");
    
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
    FILE *sessionFile;
    sessionFile = fopen("sessionFile.txt", "w+");
    fprintf(sessionFile, sessionPsw.c_str());
    fclose(sessionFile);
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
    cout << "start to encrypt message, please enter your private psw to sign the mail!" << endl;
    system(encryptShell.c_str());
    cout << "done!" << endl;
    ////////////////////////////////////////////////////////////
    fclose(sendFile);
}
void receiveEmail() {
    string emailName;
    FILE* receiveFile;
    cout << "please enter the email name you want to receive: ";
//    cin >> emailName;
    emailName = "sendFile.txt";
    receiveFile = fopen(emailName.c_str(), "r");
    while (receiveFile == NULL) {
        cout << "Sorry, there is no email named " << emailName << endl;
        cout << "please enter the email name you want to receive: ";
        cin >> emailName;
        receiveFile = fopen(emailName.c_str(), "r");
    }
    //////Verify the signature on received message/////
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int enterCount = 0;
    string test = "\n";
    cout << "size: " << sizeof(test) << endl;
    bool errorDec = true;
    while ((read = getline(&line, &len, receiveFile)) != -1) {
//        printf("Retrieved line of length %zu :\n", read);
        printf("%s", line);
        if (line[0] == '\n') {
            enterCount ++;
            cout << "enter number: " << enterCount << endl;
        }
        if (enterCount == 2) {
            errorDec = false;
            read = getline(&line, &len, receiveFile);
            string signedFN(line); //signed file name
//            char* signedFNC = signedFN.c_str();
            cout << "signed file name is: " << signedFN << endl;
//            unsigned int size = (unsigned)strlen(signedFNC);
            unsigned long nameLen = signedFN.length();
            cout << "length is: " << nameLen << endl;
            string unsignedFN = signedFN.substr(0, nameLen - 6);
            cout << "unsigned file name: " << unsignedFN << endl;
            string verifyShell = "./verifyScript.sh ";
            verifyShell += unsignedFN;
//            verifyShell += signedFN;
//            cout << "mark! " << verifyShell << endl;
////            verifyShell += " ";
//            verifyShell += unsignedFN;
//            cout << "mark! " << verifyShell << endl;
            system(verifyShell.c_str());
            break;
        }
    }
    if (errorDec) {
        cout << "the format of received file is wrong!" << endl;
        return;
    }
    
    fclose(receiveFile);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
    ///////////////////////////////////////////////////
}

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