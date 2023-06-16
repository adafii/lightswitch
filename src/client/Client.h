#pragma once

#include "client/Callback.h"
#include "ActionListener.h"
#include "observers/SwitchObserver.h"
#include <mqtt/async_client.h>
#include <string>

using std::literals::string_literals::operator ""s;
const auto CLIENT_ID = "lightswitch"s;
const auto QOS = 0;
const auto TOGGLE_LIGHT = R"({"state": "TOGGLE"})"s;

class Client {
public:
    /**
     * @param serverAddress Server address as tcp://a.b.c.d:port
     * @param switchTopic Switch topic, e.g. 'zigbee2mqtt/switch'
     * @param lightTopic Light topic, e.g. 'zigbee2mqtt/light'
     */
    Client(std::string serverAddress, std::string switchTopic, std::string lightTopic);

    ~Client();

    void toggleLight();

    void registerSwitchObserver(const SwitchObserver* switchObserver);

private:
    std::string serverAddress_{};
    std::string switchTopic_{};
    std::string lightTopic_{};
    mqtt::async_client client_;

    mqtt::connect_options connectionOptions_{};
    ActionListener subscribeListener_{"Subscribe"};
    ActionListener publishListener_{"Publish"};
    Callback callback_;
    const mqtt::message_ptr toggle_message_{};

    const SwitchObserver* switchObserver_{nullptr};

    void receiveMessage(mqtt::const_message_ptr message);
};