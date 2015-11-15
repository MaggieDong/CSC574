#!/bin/bash
sessionPsw=$1
# echo $sessionPsw
openssl enc -aes-256-cbc -base64 -in sessionFile.txt -out encryptedFile.txt -pass file:./publicKey.txt
echo >> encryptedFile.txt 
rm sessionFile.txt
openssl enc -aes-256-cbc -base64 -in message.txt -out encryptedMsg.txt -pass pass:sessionPsw
cat encryptedMsg.txt >> encryptedFile.txt 
# rm message.txt

openssl enc -d -aes-256-cbc -base64 -in encryptedMsg.txt -out original.txt -pass pass:sessionPsw
