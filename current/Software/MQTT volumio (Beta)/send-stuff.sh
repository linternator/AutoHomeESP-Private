#!/bin/bash
echo "sending MQTT volumio comand stuffs"

echo -n "enter ip :"
read ip

echo -n "enter username for that machine"
read user


#######------    fix the sending thing "user"   ----------------#############


scp -r control.js install-mqtt-RG.sh MQTT-comand,sh "user@$ip":"/home/user"	##-- send 
all relivant files to the remote machine --## 

ssh "root@$ip" chmod 777 /home/user/*  				##-- make exectable the remote setup files --##

ssh "root@$ip" sh /root/install.sh 				##-- run setup script on remote machine --##


