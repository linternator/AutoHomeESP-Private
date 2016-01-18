package com.autohome;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.io.BufferedReader;
import java.io.InputStreamReader;

/**
 * Created by david on 18/01/16.
 */
public class ShutdownCallback implements MqttCallback {

    @Override
    public void connectionLost(Throwable throwable) {



    }

    @Override
    public void messageArrived(String s, MqttMessage mqttMessage) throws Exception {

        if(mqttMessage.toString().contains("PC:POWER:")){

            String[] split = mqttMessage.toString().split(":");

            switch(Integer.parseInt(split[2])){

                case 0: {

                    System.out.println("Shutdown");

                    String[] cmd = {"/bin/bash","-c","echo \"STBCrg123\"| sudo poweroff"};
                    Process pb = Runtime.getRuntime().exec(cmd);

                    String line;
                    BufferedReader input = new BufferedReader(new InputStreamReader(pb.getInputStream()));
                    while ((line = input.readLine()) != null) {
                        System.out.println(line);
                    }
                    input.close();

                    break;

                }

                case 1: { System.out.println("Hibernate"); break; }

            }

        }

    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {

    }
}
