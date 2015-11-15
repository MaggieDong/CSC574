#!/bin/bash
sessionPsw=$1
# echo $sessionPsw
openssl enc -aes-256-cbc -base64 -in message.txt -out sendFile.txt -pass pass:sessionPsw
openssl enc -d -aes-256-cbc -base64 -in sendFile.txt -out sendFileout.txt -pass pass:sessionPsw 
