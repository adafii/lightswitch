#include "client/Client.h"
#include "client/ConnectionException.h"

Client::Client(std::string serverAddress, std::string switchTopic, std::string lightTopic)
    : serverAddress_{std::move(serverAddress)},
      switchTopic_{std::move(switchTopic)},
      lightTopic_{std::move(lightTopic)},
      client_{serverAddress_, CLIENT_ID},
      connectionOptions_{mqtt::connect_options_builder().clean_session(false).finalize()},
      callback_{client_,
                subscribeListener_,
                QOS,
                switchTopic_,
                [this](mqtt::const_message_ptr message) { receiveMessage(std::move(message)); }},
      toggle_message_{mqtt::make_message(lightTopic_ + "/set", LIGHT_TOGGLE)},
      on_message_{mqtt::make_message(lightTopic_ + "/set", LIGHT_ON)},
      off_message_{mqtt::make_message(lightTopic_ + "/set", LIGHT_OFF)} {

    try {
        client_.set_callback(callback_);
        std::cout << "Trying to connect " << serverAddress_ << '\n';
        client_.connect(connectionOptions_, nullptr, callback_);
    } catch (const mqtt::exception& exc) {
        throw ConnectionException{};
    }
}

Client::~Client() {
    try {
        client_.disconnect()->wait();
    } catch (const mqtt::exception& exc) {
        std::cerr << exc << '\n';
    }
}

void Client::lightToggle() {
    auto publishToken = client_.publish(toggle_message_, nullptr, publishListener_);
}

void Client::lightOn() {
    auto publishToken = client_.publish(on_message_, nullptr, publishListener_);
}

void Client::lightOff() {
    auto publishToken = client_.publish(off_message_, nullptr, publishListener_);
}

void Client::registerSwitchObserver(const SwitchObserver* switchObserver) {
    switchObserver_ = switchObserver;
}

void Client::receiveMessage(mqtt::const_message_ptr message) {
    if (message->to_string().find("action") != std::string::npos) {
        (*switchObserver_)([this]() { lightToggle(); });
    }
}
