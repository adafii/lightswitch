#pragma once

#include "client/Callback.h"
#include <mqtt/async_client.h>
#include <string>

class Client {
public:
    /**
     * @param serverAddress Server address as tcp://a.b.c.d:port
     * @param switchTopic Switch topic, e.g. 'zigbee2mqtt/switch'
     * @param lightTopic Light topic, e.g. 'zigbee2mqtt/light'
     */
    Client(std::string serverAddress, std::string switchTopic, std::string lightTopic);

    ~Client();

private:
    std::string serverAddress_;
    std::string switchTopic_;
    std::string lightTopic_;
    mqtt::async_client client_;
    mqtt::connect_options connectionOptions_;
    Callback callback_;
};