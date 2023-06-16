#include "client/ActionListener.h"

void ActionListener::on_failure(const mqtt::token& token) {
    std::cerr << name_ << " listener failure for token: "
              << token.get_message_id() << '\n';
}

void ActionListener::on_success(const mqtt::token& token) {
    std::cout << name_ << " listener success for token: "
              << token.get_message_id() << '\n';

    auto topics = token.get_topics();

    if (topics->empty()) {
        return;
    }

    std::cout << "- Topics: ";
    for (std::size_t i = 0; topics && i < topics->size() - 1; ++i) {
        std::cout << (*topics)[i] << ", ";
    }
    std::cout << (*topics)[topics->size() - 1] << '\n';
}