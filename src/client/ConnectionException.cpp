#include "client/ConnectionException.h"

const char* ConnectionException::what() const noexcept {
    return "Unable to connect the MQTT server";
}