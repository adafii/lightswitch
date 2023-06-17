#include "client/Client.h"
#include "client/ConnectionException.h"
#include "utils.h"
#include <atomic>
#include <csignal>
#include <mqtt/async_client.h>
#include <string>
#include <thread>

const auto TIMER_SLEEP = std::chrono::milliseconds(1000);
std::atomic_flag quit = false;

void signalHandler(int) {
    quit.test_and_set();
    quit.notify_all();
}

void lightTimer(int onHour, int offHour, std::function<void()> lightOn, std::function<void()> lightOff) {

    int currentHour = getHour();
    bool currentState = currentHour >= onHour && currentHour < offHour;
    while (!quit.test()) {
        std::this_thread::sleep_for(TIMER_SLEEP);

        currentHour = getHour();

        if (currentHour >= onHour && currentHour < offHour && currentState) {
            currentState = false;
            lightOn();
        } else if ((currentHour >= offHour || currentHour < onHour) && !currentState) {
            currentState = true;
            lightOff();
        }
    }
}

int main(int argc, char** argv) {
    auto args = getArgs(argc, argv);

    if (!args.has_value()) {
        std::cout << "Usage: " << argv[0] << " "
                  << "-a server:port -s 'switch topic' -l 'light topic' -o 'on hour' -f 'off hour'" << '\n';
        return EXIT_FAILURE;
    }

    const auto [serverAddress, switchTopic, lightTopic, onHour, offHour] = *args;

    try {
        Client mqtt_client(serverAddress, switchTopic, lightTopic);
        SwitchObserver switchObserver{};
        mqtt_client.registerSwitchObserver(&switchObserver);

        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);
        std::signal(SIGHUP, signalHandler);

        auto timer = std::jthread(
            lightTimer,
            onHour,
            offHour,
            [&mqtt_client]() { mqtt_client.lightOn(); },
            [&mqtt_client]() { mqtt_client.lightOff(); });

        quit.wait(false);

    } catch (const ConnectionException& exc) {
        std::cout << exc.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
