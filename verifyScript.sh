#!/bin/bash

openssl x509 -in pubCert.pem -pubkey -noout > public.pem
# openssl dgst -sha1 -verify public.pem -signature $signedFN $unsignedFN
perl -pi -e 'chomp if eof' encryptedFile1.txt #to remove the last new line in encryptedFile1.txt file

openssl dgst -sha1 -verify public.pem -signature encryptedFile1.txt.sha1 encryptedFile1.txt
