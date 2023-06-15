#include "client/Client.h"
#include "client/ConnectionException.h"

Client::Client(std::string serverAddress, std::string switchTopic, std::string lightTopic)
        : serverAddress_{std::move(serverAddress)},
          switchTopic_{std::move(switchTopic)},
          lightTopic_{std::move(lightTopic)},
          client_{serverAddress_, CLIENT_ID},
          connectionOptions_{mqtt::connect_options_builder().clean_session(false).finalize()},
          callback_{client_, QOS, switchTopic_, lightTopic_} {
    try {
        client_.set_callback(callback_);
        std::cout << "Trying to connect " << serverAddress_ << '\n';
        client_.connect(connectionOptions_, nullptr, callback_);
    } catch (const mqtt::exception &exc) {
        throw ConnectionException{};
    }
}

Client::~Client() {
    try {
        client_.disconnect()->wait();
    } catch (const mqtt::exception &exc) {
        std::cerr << exc << '\n';
    }
}