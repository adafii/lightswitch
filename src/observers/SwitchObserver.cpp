#include "SwitchObserver.h"

void SwitchObserver::operator()(const std::function<void()>& toggleLight) const {
    toggleLight();
}