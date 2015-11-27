#!/bin/bash
#decrypt session key using private key
########????solve pass two parameter problem
openssl rsautl -inkey RSA.pem -decrypt -in encSess.enc -out sessionkey.txt
echo "mark!"
#message encrypted under session pwd above
openssl enc -d -aes-256-cbc -base64 -in sendFileMsg.txt -pass file:sessionkey.txt
#-out decryptedMsg.txt

#remove mid-file generated
rm encSess.enc
rm sessionkey.txt
rm sendFileMsg.txt

#remove the email after reading
# rm sendFile.txt