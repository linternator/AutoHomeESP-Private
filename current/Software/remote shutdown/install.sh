#!/bin/bash
echo "starting install"

apt-get install python-pip
echo "installing 1"
apt-get install python-dev
echo "installing mqtt bit"
pip install paho-mqtt

chmod +x shutdownsMQTT.py

cp remote-shutdown.service /etc/systemd/system
cp shutdownsMQTT.py /usr/local/bin/

systemctl enable remote-shutdown.service
systemctl remote-shutdown.service

echo "done"
