#!/bin/bash
echo "starting install"

apt-get install python-pip
echo "installing 1"
apt-get install python-dev
echo "installing mqtt bit"
pip install paho-mqtt

echo -n "enter pc name :"
read nam

chmod +x shutdownsMQTT.py

sed -i "s/%CHANGE-ME%/$nam/g" shutdownsMQTT.py

cp remote-shutdown.service /etc/systemd/system
cp shutdownsMQTT.py /usr/local/bin/

systemctl enable remote-shutdown.service

rm remote-shutdown.service
rm shutdownsMQTT.py
rm install.sh

echo "done"

reboot now
