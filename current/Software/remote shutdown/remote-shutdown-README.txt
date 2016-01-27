make sure i have python.

sudo apt-get install python-pip
suod apt-get install python-dev

sudo pip install paho-mqtt

import paho.mqtt.client as mqtt

run shutdownMQTT.py from somewhere

////////////////////////////
eddit on the shutdownMQTT.py

client.subscribe("/home/computer/IBMlaptop/") 
to
client.subscribe("/home/computer/"your pc name"/")

scp /home/ ect /remote-shutdown.service "user"@"ip":
scp /home/ ect /shutdownsMQTT.py "user"@"ip":

// ssh into the pi

chmod +x shutodwnMQTT.py

sudo cp remote-shutdown.service /etc/systemd/system/  
sudo cp shutdownMQTT.py /usr/local/bin/

sudo systemctl enable remote-shutdown.service
sudo systemctl start remote-shutdown.service

