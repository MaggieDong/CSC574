#!/bin/bash
#decrypt session key using private key
########????solve pass two parameter problem
privateKey=$1
openssl rsautl -inkey RSA.pem -decrypt -in encSess.enc
# openssl enc -d -aes-256-cbc -base64 -in sendFileSesK.txt -out sessionkey.txt -pass pass:$privateKey

# echo >> encryptedFile.txt
# rm sessionFile.txt

# #message encrypted under session pwd above
# openssl enc -d -aes-256-cbc -base64 -in message.txt -out encryptedMsg.txt -pass pass:sessionPsw
# cat encryptedMsg.txt >> encryptedFile.txt
# echo >> encryptedFile.txt
# rm message.txt
# rm encryptedMsg.txt
#
# # openssl enc -d -aes-256-cbc -base64 -in encryptedMsg.txt -out original.txt -pass pass:sessionPsw
#
# #signature of above content
# openssl dgst -sha1 -sign RSA.pem -out encryptedFile.txt.sha1 encryptedFile.txt
#
# #Format and send
# echo "-----BEGIN CSC474 MESSAGE-----" >> sendFile.txt
# cat encryptedFile.txt >> sendFile.txt
# echo "encryptedFile.txt.sha1" >> sendFile.txt
# echo "-----END CSC474 MESSAGE-----" >> sendFile.txt