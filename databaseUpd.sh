#!/bin/bash
receiver=$1
database=database.txt
if grep -q $receiver "$database"; then #extract the pem file
	echo "exist in database!"
	line=$(grep -i $receiver database.txt)
	IFS=',' read -r -a recv <<< "$line"
	echo ${recv[0]}
	echo ${recv[1]}
	fileN=${recv[1]}
	openssl verify -verbose -CAfile $fileN root-ca.crt #> verifyCARes.txt
	
else #get the pem file from course webpage
	echo "not found in database!"
	if grep -q $receiver "CertificateRepo.txt"; then #find receiver in CertificateRepo.txt
		echo "found in CertificateRepo.txt"
		line=$(grep -i $receiver CertificateRepo.txt)
		IFS=',' read -r -a link <<< "$line"
		webLink=${link[1]}
		echo $webLink
		IFS='/' read -r -a indx <<< "$webLink"
		fileN=${indx[7]}
		echo $fileN
		curl -o $fileN $webLink #download the pem file to the local directory
		echo $receiver","$fileN >> database.txt 
		openssl verify -verbose -CAfile $fileN root-ca.crt #> verifyCARes.txt
	else
		echo "not found receiver ID in CertificateRepo.txt"
	fi
fi