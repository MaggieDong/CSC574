#!/bin/bash
privKeyFile=$1
#decrypt session key using private key
########????solve pass two parameter problem
openssl rsautl -inkey $privKeyFile -decrypt -in encSess1.enc -out sessionkey.txt
echo "The decrypted message is:"
#message encrypted under session pwd above
openssl enc -d -aes-256-cbc -base64 -in sendFileMsg.txt -pass file:sessionkey.txt
#-out decryptedMsg.txt

#remove mid-file generated
rm encSess1.enc
rm sessionkey.txt
rm sendFileMsg.txt
rm pemFileN.txt
rm verifyCARes.txt
rm encryptedFile1.txt
rm encryptedFile1.txt.sha1

#remove the email after reading
# rm sendFile.txt