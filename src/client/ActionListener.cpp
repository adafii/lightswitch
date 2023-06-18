#include "client/ActionListener.h"

void ActionListener::on_failure(const mqtt::token& token) {
    std::cerr << name_ << " listener failure for token: " << token.get_message_id() << '\n';
}

void ActionListener::on_success(const mqtt::token&) {
}