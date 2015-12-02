#!/bin/bash
privKeyFile=$1
#session pwd encrypted under target's public key
openssl x509 -in pubCert.pem -pubkey -noout > public.pem #get receiver's public key
openssl rsautl -encrypt -inkey public.pem -pubin -in sessionFile.txt -out encSess.enc
# base64 -d encSess.enc > encryptedFile.txt
# echo "encSess.enc" > encryptedFile.txt
cat encSess.enc > encryptedFile.txt
echo >> encryptedFile.txt
echo >> encryptedFile.txt


#message encrypted under session pwd above
openssl enc -aes-256-cbc -base64 -in message.txt -out encryptedMsg.txt -pass file:sessionFile.txt
perl -pi -e 'chomp if eof' encryptedMsg.txt #to remove the last new line in encryptedMsg.txt file
cat encryptedMsg.txt >> encryptedFile.txt 
# echo >> encryptedFile.txt

# openssl enc -d -aes-256-cbc -base64 -in encryptedMsg.txt -out original.txt -pass file:sessionFile.txt

#signature of above content
openssl dgst -sha1 -sign $privKeyFile -out encryptedFile.txt.sha1 encryptedFile.txt
#Format and send
# echo "" >>sendFile.txt
echo "-----BEGIN CSC574 MESSAGE-----" >> sendFile.txt
cat encryptedFile.txt >> sendFile.txt
echo >> sendFile.txt
echo >> sendFile.txt
# echo "encryptedFile.txt.sha1" >> sendFile.txt
cat encryptedFile.txt.sha1 >> sendFile.txt
echo >> sendFile.txt
echo "-----END CSC574 MESSAGE-----" >> sendFile.txt

# rm message.txt
# rm encryptedMsg.txt
# rm sessionFile.txt

# openssl dgst -sha1 -verify public.pem -signature encryptedFile.txt.sha1 encryptedFile.txt
