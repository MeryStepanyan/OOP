#pragma once
#include <string>
#include <unordered_map>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
};
