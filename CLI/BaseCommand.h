
#pragma once
#include "ICommand.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>


class ISlideRepository;

class BaseCommand : public ICommand {
protected:
    std::unordered_map<std::string, std::string> arguments;

    
    bool hasArgument(const std::string& key) const;
    const std::string& getArgumentValue(const std::string& key) const;
    

    int parseIntArgument(const std::string& key, int defaultValue = 0) const;
    size_t parseSizeTArgument(const std::string& key, size_t defaultValue = 0) const;
    
    void validateAllowedArguments(const std::vector<std::string>& allowed) const;
    
 
    void validateRequiredArgument(const std::string& key) const;

public:
    BaseCommand() = default;
    explicit BaseCommand(const std::unordered_map<std::string, std::string>& args)
        : arguments(args) {
    }
    
    virtual ~BaseCommand() = default;
};
