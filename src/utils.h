#pragma once
#include <optional>
#include <string>
#include <tuple>
#include <unistd.h>

std::optional<std::tuple<std::string, std::string, std::string>> getArgs(int argc, char** argv) {
    std::string serverAddress{};
    std::string switchTopic{};
    std::string lightTopic{};

    int option = getopt(argc, argv, "a:s:l:");
    while (option != -1) {
        switch (option) {
            case 'a':
                serverAddress = "tcp://" + std::string(optarg);
                break;
            case 's':
                switchTopic = std::string(optarg);
                break;
            case 'l':
                lightTopic = std::string(optarg);
                break;
        }
        option = getopt(argc, argv, "s:b:l:");
    }

    if (serverAddress.empty() || switchTopic.empty() || lightTopic.empty()) {
        return std::nullopt;
    }

    return {{serverAddress, switchTopic, lightTopic}};
}