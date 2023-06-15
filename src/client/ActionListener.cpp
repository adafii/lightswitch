#include "client/ActionListener.h"

void ActionListener::on_failure(const mqtt::token& token) {
    std::cerr << name_ << " failed\n";
}

void ActionListener::on_success(const mqtt::token& token) {
    std::cout << name_ << " succeeded for topics: \n";
    auto topics = token.get_topics();

    for (std::size_t i = 0; topics && i < topics->size(); ++i) {
        std::cout << '\t' << (*topics)[i] << '\n';
    }
}