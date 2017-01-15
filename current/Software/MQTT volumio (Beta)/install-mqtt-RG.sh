#!/bin/bash

echo "installing MQTT comands for volumio (or any raspbery pi based meda thing)"

apt-get update

mkdir /MQTT-things

 cp control.js /MQTT-things/
 cp startupthings /MQTT-things/
 cp MQTT-comand.sh /MQTT-things/

cd /MQTT-things ##-- do i need this before the install of node.js ? 

echo "begging install of node js"

apt-get install nodejs

apt-get install npm

echo "installing mqtt client"
npm install mqtt

npm install socket.io-client

echo "adding to start up list"

 sudo cp mqtt-comand.sh  /etc/init.d/
 sudo chmod 755 /etc/init.d/MQTT-comand.sh
 sudo update-rc.d MQTT-comand.sh defaults 
 
echo "done, enjoy"
