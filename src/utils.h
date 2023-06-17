#pragma once

#include <optional>
#include <string>
#include <tuple>
#include <unistd.h>

std::optional<std::tuple<std::string, std::string, std::string, int, int>> getArgs(int argc, char** argv) {
    std::string serverAddress{};
    std::string switchTopic{};
    std::string lightTopic{};
    int onHour = -1;
    int offHour = -1;

    const auto arguments = "a:s:l:o:f:";

    int option = getopt(argc, argv, arguments);
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
            case 'o':
                onHour = std::atoi(optarg);
                break;
            case 'f':
                offHour = std::atoi(optarg);
                break;
        }
        option = getopt(argc, argv, arguments);
    }

    if (serverAddress.empty() || switchTopic.empty() || lightTopic.empty() || onHour == -1 || offHour == -1) {
        return std::nullopt;
    }

    return {{serverAddress, switchTopic, lightTopic, onHour, offHour}};
}

int getHour() {
    auto current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto time_info = std::localtime(&current_time);
    return time_info->tm_hour;
}