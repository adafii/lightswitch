#pragma once

#include <functional>
#include <iostream>

class SwitchObserver {
public:
    void operator()(const std::function<void()>& toggleLight) const;
};