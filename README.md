Lightswitch
===========

Simple program for controlling lights over MQTT. The hardware consists of a led light, zigbee relay, zigbee switch, and an arm single board computer with a zigbee adapter. The program communicates with a MQTT broker to receive messages from the zigbee switch and then sends messages to the zigbee relay to toggle the light on or off. In addition, the program can control the light according to an user set schedule.

![image](https://github.com/adafii/lightswitch/assets/136560098/208fde95-5217-4eed-ae0c-842b236b662c)

Building
--------

Building requires [Eclipse Paho MQTT C++ Client Library](https://github.com/eclipse/paho.mqtt.cpp/).
```
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/lightswitch
```
