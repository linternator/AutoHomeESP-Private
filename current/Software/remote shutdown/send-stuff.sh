#!/bin/bash
echo "sending shutdown stuff"

echo -n "enter ip :"
read ip

scp -r remote-shutdown.service shutdownsMQTT.py install.sh "root@$ip":"/root/"

ssh "root@$ip" chmod 777 /root/install.sh 
ssh "root@$ip" sh /root/install.sh

