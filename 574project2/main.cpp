//
//  main.cpp
//  574project2
//
//  Created by mdong3 on 11/13/15.
//  Copyright (c) 2015 mdong3. All rights reserved.
//

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
    cin >> choice;
//    choice = 1;
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
//    cout << "please input the email name: ";
    string emailName = "sendFile";
//    cin >> emailName;
    sendFile = fopen(emailName.c_str(), "w");
    
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
    cout << "session key is: " << sessionPsw << endl;
    FILE *sessionFile;
    sessionFile = fopen("sessionFile.txt", "w");
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
    string encryptShell = "./encryptMes.sh";
    cout << "start to encrypt message, please enter your private psw to sign the mail!" << endl;
    system(encryptShell.c_str());
    cout << "done!" << endl;
    ////////////////////////////////////////////////////////////
    fclose(sendFile);
}
void receiveEmail() {
    string emailName;
    FILE* receiveFile;
//    cout << "please enter the email name you want to receive: ";
//    cin >> emailName;
    emailName = "sendFile";
    string mailName = emailName + ".txt";
//    cout << "email name is: " << mailName << endl;
    receiveFile = fopen(mailName.c_str(), "r");
    while (receiveFile == NULL) {
        cout << "Sorry, there is no email named " << mailName << endl;
        cout << "please enter the email name you want to receive: ";
        cin >> mailName;
        receiveFile = fopen(mailName.c_str(), "r");
    }
    int verifyRes = -1;
    //////Verify the signature on received message/////
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int enterCount = 0;
    string test = "\n";
//    cout << "size: " << sizeof(test) << endl;
    bool errorDec = true;
    while ((read = getline(&line, &len, receiveFile)) != -1) {
//        printf("Retrieved line of length %zu :\n", read);
//        printf("%s", line);
        if (line[0] == '\n') {
            enterCount ++;
//            cout << "enter number: " << enterCount << endl;
        }
        if (enterCount == 2) {
            errorDec = false;
            read = getline(&line, &len, receiveFile);
            string signedFN(line); //signed file name
//            char* signedFNC = signedFN.c_str();
//            cout << "signed file name is: " << signedFN << endl;
//            unsigned int size = (unsigned)strlen(signedFNC);
            unsigned long nameLen = signedFN.length();
//            cout << "length is: " << nameLen << endl;
            string unsignedFN = signedFN.substr(0, nameLen - 6);
//            cout << "unsigned file name: " << unsignedFN << endl;
            string verifyShell = "./verifyScript.sh ";
            verifyShell += unsignedFN;
//            verifyShell += signedFN;
//            cout << "mark! " << verifyShell << endl;
////            verifyShell += " ";
//            verifyShell += unsignedFN;
//            cout << "mark! " << verifyShell << endl;
            verifyRes = system(verifyShell.c_str());
//            cout << "verify or not? " << verifyRes << endl;
            break;
        }
    }
    fclose(receiveFile);
    if (line)
        free(line);
//    exit(EXIT_SUCCESS);
    
    if (errorDec) {
        cout << "the format of received file is wrong!" << endl;
        return;
    }
    ///////////////////////////////////////////////////
    
    ////////decrypt session key with private key///////
    if (verifyRes == 0) { //if Verification OK
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        int lineCount = 0;
//        string test = "\n";
//        cout << "size: " << sizeof(test) << endl;
//        bool errorDec = true;
        FILE * sessionKey;
        string sessionKeyFN = emailName + "SesK.txt";
        sessionKey = fopen(sessionKeyFN.c_str(), "w");
        receiveFile = fopen(mailName.c_str(), "r");
        while ((read = getline(&line, &len, receiveFile)) != -1) {
//            printf("Retrieved line of length %zu :\n", read);
//            printf("%s", line);
            lineCount ++;
            if (line[0] != '\n' && lineCount > 1) {
//                cout << "seccess!" << endl;
                fprintf(sessionKey, line);
            }
            if (line[0] == '\n') {
                break;
            }
        }
        fclose(receiveFile);
        fclose(sessionKey);
        if (line)
            free(line);
//        string privateKey;
//        cout << "please enter your private key to decrypt message: ";
////        cin >> privateKey;
//        privateKey = "RSA09120705ym";
//        string decryptShell = "./decryptShell.sh ";
//        decryptShell += privateKey;
//        system(decryptShell.c_str());
    }
    ///////////////////////////////////////////////////
    
    //////////decrypt message using session key////////
    if (verifyRes == 0) { //if Verification OK
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        int newLineCount = 0;
        FILE * message;
        string messageFN = emailName + "Msg.txt";
        message = fopen(messageFN.c_str(), "w");
        receiveFile = fopen(mailName.c_str(), "r");
        while ((read = getline(&line, &len, receiveFile)) != -1) {
//            printf("Retrieved line of length %zu :\n", read);
//            printf("%s", line);
            if (line[0] != '\n' && newLineCount == 1) {
//                cout << "seccess!" << endl;
                fprintf(message, line);
            }
            if (line[0] == '\n') {
                newLineCount ++;
                if (newLineCount == 2)
                    break;
            }
        }
        fclose(receiveFile);
        fclose(message);
        if (line)
            free(line);
        string decryptShell = "./decryptShell.sh";
        system(decryptShell.c_str());
    }
    ///////////////////////////////////////////////////
}