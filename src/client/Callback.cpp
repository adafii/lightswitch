#include "client/Callback.h"
#include "client/ConnectionException.h"

Callback::Callback(mqtt::async_client &client, int qos, std::string switchTopic, std::string lightTopic)
        : client_{client},
          qos_{qos},
          switchTopic_{std::move(switchTopic)},
          lightTopic_{std::move(lightTopic)},
          toggle_message{mqtt::make_message(lightTopic_ + "/set", TOGGLE_LIGHT)} {};

void Callback::reconnect() {
    std::this_thread::sleep_for(RECONNECT_TIMEOUT);
    try {
        client_.reconnect();
    } catch (const mqtt::exception &exc) {
        std::cerr << "Error: " << exc.what() << '\n';
        exit(EXIT_FAILURE);
    }
}

void Callback::on_failure(const mqtt::token &) {
    std::cerr << "Connection attempt failed\n";
    ++retries_;
    if (retries_ > RECONNECT_ATTEMPTS) {
        std::cerr << "Maximum attempts tried, aborting...\n";
        exit(EXIT_FAILURE);
    }
    reconnect();
}

void Callback::on_success(const mqtt::token &) {
}

void Callback::connected(const std::string & /*cause*/) {
    std::cout << "Connection succeeded\n";
    client_.subscribe(switchTopic_, qos_, nullptr, subscriptionListener_);
}

void Callback::connection_lost(const std::string &cause) {
    std::cout << "Connection lost\n";
    if (!cause.empty()) {
        std::cout << " Cause: " << cause;
    }
    std::cout << '\n';

    std::cout << "Reconnecting...\n";
    retries_ = 0;
    reconnect();
}

void Callback::message_arrived(mqtt::const_message_ptr message) {
    if (message->to_string().find("action") != std::string::npos) {
        auto publishToken = client_.publish(toggle_message, nullptr, publishListener_);
    }
}

void Callback::delivery_complete(mqtt::delivery_token_ptr token) {
    std::cout << "\tDelivery complete for token: "
              << (token ? token->get_message_id() : -1) << '\n';
}