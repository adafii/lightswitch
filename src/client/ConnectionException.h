#pragma once

#include <exception>
#include <format>
#include <string>

class ConnectionException : public std::exception {
public:
    const char* what() const noexcept override;
};