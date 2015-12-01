#!/bin/bash
privKeyFile=$1
#decrypt session key using private key
########????solve pass two parameter problem
openssl rsautl -inkey $privKeyFile -decrypt -in encSess.enc -out sessionkey.txt
echo "The decrypted message is:"
#message encrypted under session pwd above
openssl enc -d -aes-256-cbc -base64 -in sendFileMsg.txt -pass file:sessionkey.txt
#-out decryptedMsg.txt

#remove mid-file generated
rm encSess.enc
rm sessionkey.txt
rm sendFileMsg.txt
# rm Msg.txt
rm pemFileN.txt
rm sendFileSesK.txt
# rm SesK.txt
rm verifyCARes.txt
rm encryptedFile.txt
#remove the email after reading
# rm sendFile.txt