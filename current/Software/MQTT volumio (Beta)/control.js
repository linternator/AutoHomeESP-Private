var mqttTopic = "/garage/radio"

var io = require('socket.io-client');
var mqtt = require('mqtt')

var socket = io.connect('http://localhost:3000', {reconnect: true});
var client  = mqtt.connect('mqtt://192.168.1.162', {username:'autohome', password:'password'})

function IsJsonString(str) {
    try {
        JSON.parse(str);
    } catch (e) {
        return false;
    }
    return true;
}

client.on('connect', function () {

  console.log("MQTT Connected")

  client.subscribe(mqttTopic + "/command")

})

client.on('message', function (topic, message) {
  
  var split = message.toString().split("||")

  console.log(split);  

  if(split.length == 2){
    if(!isNaN(split[1])){ 
        socket.emit(split[0], Number(split[1]))
    } else if(IsJsonString(split[1])){
        socket.emit(split[0], JSON.parse(split[1]))
    } else {
         socket.emit(split[0], split[1])
    }
  }
 
  if(split.length == 1){

    socket.emit(split[0])

  }
})

socket.on('connect', function () {

  console.log('Client Connected');

});

socket.on('pushState', function(data){

  client.publish(mqttTopic + "/getState", JSON.stringify(data))

})

socket.on('pushListPlaylist', function(data){

  client.publish(mqttTopic + "/playlist", JSON.stringify(data))

})

socket.on('pushCreatePlaylist', function(data){

  client.publish(mqttTopic + "/playlist", JSON.stringify(data))

})

socket.on('pushBrowseLibrary', function(data){

  client.publish(mqttTopic + "/library", JSON.stringify(data))

})

