#pragma once

#include <mqtt/async_client.h>
#include <string>

class ActionListener : public mqtt::iaction_listener {
public:
    explicit ActionListener(std::string name) : name_{std::move(name)} {
    }

private:
    std::string name_;

    void on_failure(const mqtt::token &token) override;

    void on_success(const mqtt::token &token) override;
};