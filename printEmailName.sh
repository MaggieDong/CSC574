#!/bin/bash
certificateN=$1
line=$(grep -i "Subject:" $certificateN)
IFS='=' read -r -a emailAdd <<< "$line"
address=${emailAdd[7]}
echo "sender name is: "$address	