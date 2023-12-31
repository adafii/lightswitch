#include "client/Callback.h"

Callback::Callback(mqtt::async_client& client,
                   ActionListener& subscribeListener,
                   int qos,
                   std::string switchTopic,
                   std::function<void(mqtt::const_message_ptr)> notifyMessage)
    : client_{client},
      subscribeListener_{subscribeListener},
      qos_{qos},
      switchTopic_{std::move(switchTopic)},
      notifyMessage_{std::move(notifyMessage)} {
}

void Callback::reconnect() {
    ++retries_;
    if (retries_ > RECONNECT_ATTEMPTS) {
        std::cerr << "Maximum reconnection attempts reached, aborting...\n";
        exit(EXIT_FAILURE);
    }

    std::this_thread::sleep_for(RECONNECT_TIMEOUT);

    try {
        client_.reconnect();
    } catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << '\n';
        exit(EXIT_FAILURE);
    }
}

void Callback::on_failure(const mqtt::token&) {
    std::cerr << "Connection attempt failed\n";
    reconnect();
}

void Callback::on_success(const mqtt::token&) {
}

void Callback::connected(const std::string& /*cause*/) {
    std::cout << "Connection succeeded\n";
    client_.subscribe(switchTopic_, qos_, nullptr, subscribeListener_);
}

void Callback::connection_lost(const std::string& cause) {
    std::cout << "Connection lost\n";
    if (!cause.empty()) {
        std::cout << " Cause: " << cause << '\n';
    }

    std::cout << "Reconnecting...\n";
    reconnect();
}

void Callback::message_arrived(mqtt::const_message_ptr message) {
    notifyMessage_(message);
}

void Callback::delivery_complete(mqtt::delivery_token_ptr token) {
    std::cout << "\tDelivery complete for token: " << (token ? token->get_message_id() : -1) << '\n';
}
