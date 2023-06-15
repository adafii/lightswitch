#include "client/Client.h"
#include "client/ConnectionException.h"
#include "utils.h"
#include <csignal>
#include <mqtt/async_client.h>
#include <string>

const auto THREAD_SLEEP = std::chrono::microseconds(1000);
std::atomic<bool> quit(false);

void sigInt(int) {
    quit = true;
}

int main(int argc, char** argv) {
    auto args = getArgs(argc, argv);

    if (!args.has_value()) {
        std::cout << "Usage: " << argv[0] << " "
                  << "-a server:port -s 'switch topic' -l 'light topic'" << '\n';
        return EXIT_FAILURE;
    }

    const auto [serverAddress, switchTopic, lightTopic] = *args;

    try {
        Client mqtt_client(serverAddress, switchTopic, lightTopic);

        std::signal(SIGINT, sigInt);

        while (true) {
            std::this_thread::sleep_for(THREAD_SLEEP);
            if (quit.load()) {
                break;
            }
        }
    } catch (const ConnectionException& exc) {
        std::cout << exc.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}