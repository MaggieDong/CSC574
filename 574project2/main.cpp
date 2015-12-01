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
    string databaseN = "database.txt";
    FILE* databaseF;
    databaseF = fopen(databaseN.c_str(), "a+");
    fclose(databaseF);
    
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
//    FILE* sendFile;
////    cout << "please input the email name: ";
//    string emailName = "sendFile.txt";
////    cin >> emailName;
//    sendFile = fopen(emailName.c_str(), "w");
    cout << "pleas enter your email ID: " ;
    string sender, receiver;
    cin >> sender; //sender's ID
    cout << "please enter the destination email ID: ";
    cin >> receiver; //receiver's ID
    cout << endl;
    
    FILE* sendFile;
    //    cout << "please input the email name: ";
    string emailName = "sendFile.txt";
    //    cin >> emailName;
    sendFile = fopen(emailName.c_str(), "w");

    fprintf(sendFile, "from: %s, to: %s\n", sender.c_str(), receiver.c_str());
    fclose(sendFile);
    
    //////////////verify receiver's cert//////////////////
    string dbShell = "./databaseUpd.sh ";
    
    dbShell += receiver;
    int CApass = -1;
    CApass = system(dbShell.c_str());
    if (CApass == 0) {
        cout << "Verified receiver's certificate!" << endl;
    }
    //////////////////////////////////////////////////////
    
    if (CApass == 0) {
        ///////////get the receiver's pem file name////////////////
        FILE* pemFileN = fopen("pemFileN.txt", "r");
        char *pemFNline = NULL;
        size_t len = 0;
        ssize_t fileNline;
        fileNline = getline(&pemFNline, &len, pemFileN);
        fclose(pemFileN);
        string PemFileName(pemFNline);
        PemFileName = PemFileName.substr(0, 8);
        ////////////////////////////////////////////////
        
        ///////get the receiver's public key//////////
        FILE * certFile;
        FILE * pubCert;
        certFile = fopen(PemFileName.c_str(), "r");
        char * LINE = NULL;
        size_t length = 0;
        ssize_t readLine;
        pubCert = fopen("pubCert.pem","w");
        while ((readLine = getline(&LINE, &length, certFile)) != -1) {
            int begin = strcmp(LINE,"-----BEGIN CERTIFICATE-----\n");
            
            if(begin == 0){
                while (strcmp(LINE,"-----END CERTIFICATE-----\n")!=0) {
                    fputs (LINE,pubCert);
                    getline(&LINE, &length, certFile);
                }
                fputs (LINE,pubCert);
                fclose (pubCert);
                break;
            }
        }
        fclose(certFile);
        //    string senderPubShell = "./senderPub.sh";
        //    system(senderPubShell.c_str());
        ///////////////////////////////////////////////
        
        ////////////////Generate 32 characters string as the session key/////////////////
        int secIndx;
        string sessionPsw;
        
        /* initialize random seed: */
        srand( 62716/*static_cast<unsigned int>(time(NULL))*/);
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
//        cout << "session key is: " << sessionPsw << endl;
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
        cout << "please enter the file name of your private key file(X.pem): " << endl;
        string privKeyFile;
        cin >> privKeyFile;
        string encryptShell = "./encryptMes.sh ";
        encryptShell += privKeyFile;
        cout << "start to encrypt message, please enter your private psw to sign the mail!" << endl;
        system(encryptShell.c_str());
        cout << "send!" << endl;
        ////////////////////////////////////////////////////////////
    }
}

void receiveEmail() {
    string emailName;
    FILE* receiveFile;
//    cout << "please enter the email name you want to receive: ";
//    cin >> emailName;
//    emailName = "sendFile";
    string mailName = "sendFile.txt";//emailName + ".txt";
//    cout << "email name is: " << mailName << endl;
    receiveFile = fopen(mailName.c_str(), "r");
    while (receiveFile == NULL) {
        cout << "Sorry, there is no email to read!" << endl;
//        cout << "Sorry, there is no email named " << mailName << endl;
//        cout << "please enter the email name you want to receive: ";
//        cin >> mailName;
//        receiveFile = fopen(mailName.c_str(), "r");
    }
    //////obtain sender's email address from mail header//////
    string sender = "";
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    if ((read = getline(&line, &len, receiveFile)) != -1) {
        for (int i = 6; i < strlen(line); i++) {
            if (line[i] == ',') {
                break;
            }
            sender += line[i];
        }
    }
//    cout << "test sender name: " << sender << endl;
    //////////////////////////////////////////////////////////
    
    //////////////verify sender's cert//////////////////
    string dbShell = "./databaseUpd.sh ";
    dbShell += sender;
    int CApass = -1;
    CApass = system(dbShell.c_str());
    if (CApass == 0) {
        cout << "Verified sender's certificate!" << endl;
    }
    //////////////////////////////////////////////////////
    
    if (CApass == 0) {
        ///////////get the pem file name////////////////
        FILE* pemFileN = fopen("pemFileN.txt", "r");
        char *pemFNline = NULL;
        size_t len = 0;
        ssize_t fileNline;
        fileNline = getline(&pemFNline, &len, pemFileN);
        fclose(pemFileN);
        string PemFileName(pemFNline);
        PemFileName = PemFileName.substr(0, 8);
        ////////////////////////////////////////////////
        
        ///////print sender name from certificate//////
        string printNameShell = "./printEmailName.sh ";
        printNameShell += PemFileName;
        system(printNameShell.c_str());
        ///////////////////////////////////////////////
        
        ///////get the target's public key//////////
        FILE * certFile;
        FILE * pubCert;
        certFile = fopen(PemFileName.c_str(), "r");
        char * LINE = NULL;
        size_t length = 0;
        ssize_t readLine;
        pubCert = fopen("pubCert.pem","w");
        while ((readLine = getline(&LINE, &length, certFile)) != -1) {
            int begin = strcmp(LINE,"-----BEGIN CERTIFICATE-----\n");
            
            if(begin == 0){
                while (strcmp(LINE,"-----END CERTIFICATE-----\n")!=0) {
                    fputs (LINE,pubCert);
                    getline(&LINE, &length, certFile);
                }
                fputs (LINE,pubCert);
                fclose (pubCert);
                break;
            }
        }
        fclose(certFile);
        ///////////////////////////////////////////////
        
        //////Verify the signature on received message/////
        receiveFile = fopen(mailName.c_str(), "r");
        int verifyRes = -1;

        line = NULL;
        len = 0;
        int enterCount = 0;
    //    string test = "\n";
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
            string sessionKeyFN = "sendFileSesK.txt";
            sessionKey = fopen(sessionKeyFN.c_str(), "w");
            receiveFile = fopen(mailName.c_str(), "r");
            while ((read = getline(&line, &len, receiveFile)) != -1) {
    //            printf("Retrieved line of length %zu :\n", read);
    //            printf("%s", line);
                lineCount ++;
                if (line[0] != '\n' && lineCount > 2) {
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
            string messageFN = "sendFileMsg.txt";
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
            cout << "please enter the file name of your private key file(X.pem): " << endl;
            string privKeyFile;
            cin >> privKeyFile;
            string decryptShell = "./decryptShell.sh ";
            decryptShell += privKeyFile;
            system(decryptShell.c_str());
        }
        ///////////////////////////////////////////////////
    }
}
