#!/bin/bash
unsignedFN=$1
signedFN=$unsignedFN".sha1"
# unsignedFN=$2
echo $signedFN
echo $unsignedFN

openssl dgst -sha1 -verify public.pem -signature $signedFN $unsignedFN
# openssl dgst -sha1 -verify public.pem -signature encryptedFile.txt.sha1 encryptedFile.txt