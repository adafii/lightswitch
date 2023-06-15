#pragma once

#include "client/ActionListener.h"
#include <chrono>
#include <mqtt/async_client.h>

using std::string_literals::operator ""s;

const auto RECONNECT_TIMEOUT = std::chrono::milliseconds(2000);
const auto RECONNECT_ATTEMPTS = 3;
const auto TOGGLE_LIGHT = R"({"state": "TOGGLE"})"s;

class Callback : public mqtt::callback, public mqtt::iaction_listener {
public:
    /**
     * @param client Paho mqtt client
     * @param connectOptions Paho mqtt connection options
     * @param qos Quality of service
     * @param switchTopic Zigbee2mqtt switch topic
     */
    Callback(mqtt::async_client &client, int qos, std::string switchTopic, std::string lightTopic);

private:
    mqtt::async_client &client_;
    int retries_{0};
    ActionListener subscriptionListener_{"Subscription"s};
    ActionListener publishListener_{"Publish"s};
    int qos_{0};
    const std::string switchTopic_{};
    const std::string lightTopic_{};
    const mqtt::message_ptr toggle_message{};

    void reconnect();

    void on_failure(const mqtt::token &token) override;

    void on_success(const mqtt::token &tok) override;

    void connected(const std::string &cause) override;

    void connection_lost(const std::string &cause) override;

    void message_arrived(mqtt::const_message_ptr message) override;

    void delivery_complete(mqtt::delivery_token_ptr token) override;
};