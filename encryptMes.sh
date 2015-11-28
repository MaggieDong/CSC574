#!/bin/bash
#session pwd encrypted under target's public key
openssl rsautl -encrypt -inkey public.pem -pubin -in sessionFile.txt -out encSess.enc
# base64 -d encSess.enc > encryptedFile.txt
echo "encSess.enc" > encryptedFile.txt
echo >> encryptedFile.txt

#message encrypted under session pwd above
openssl enc -aes-256-cbc -base64 -in message.txt -out encryptedMsg.txt -pass file:sessionFile.txt
cat encryptedMsg.txt >> encryptedFile.txt 
echo >> encryptedFile.txt

rm message.txt
rm encryptedMsg.txt
rm sessionFile.txt

# openssl enc -d -aes-256-cbc -base64 -in encryptedMsg.txt -out original.txt -pass file:sessionFile.txt

#signature of above content
openssl dgst -sha1 -sign RSA.pem -out encryptedFile.txt.sha1 encryptedFile.txt

#Format and send
# echo "" >>sendFile.txt
echo "-----BEGIN CSC474 MESSAGE-----" >> sendFile.txt
cat encryptedFile.txt >> sendFile.txt
echo "encryptedFile.txt.sha1" >> sendFile.txt
echo "-----END CSC474 MESSAGE-----" >> sendFile.txt



# openssl dgst -sha1 -verify public.pem -signature encryptedFile.txt.sha1 encryptedFile.txt
