#!/bin/bash
echo "sending shutdown stuff"

scp /home ect /remote-shutdown.service "user@ip"
scp /home ect /shutdownMQTT.py "user@ip" 
scp /home ect /install.sh "user@ip"

echo "done, dont foget to change the (client.subscribbe /home/coputer/ your pc name )"


