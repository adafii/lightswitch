#pragma once

#include <exception>
#include <string>

class ConnectionException : public std::exception {
public:
    const char* what() const noexcept override;
};