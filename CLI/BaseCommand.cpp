
#include "BaseCommand.h"
#include <algorithm>
#include <sstream>
#include <vector>

bool BaseCommand::hasArgument(const std::string& key) const {
    return arguments.find(key) != arguments.end();
}

const std::string& BaseCommand::getArgumentValue(const std::string& key) const {
    auto it = arguments.find(key);
    if (it == arguments.end()) {
        throw std::invalid_argument("Required argument not found: " + key);
    }
    return it->second;
}

int BaseCommand::parseIntArgument(const std::string& key, int defaultValue) const {
    auto it = arguments.find(key);
    if (it == arguments.end()) {
        return defaultValue;
    }
    
    try {
        return std::stoi(it->second);
    }
    catch (const std::exception&) {
        throw std::invalid_argument("Argument '" + key + "' must be a valid integer: " + it->second);
    }
}

size_t BaseCommand::parseSizeTArgument(const std::string& key, size_t defaultValue) const {
    int value = parseIntArgument(key, static_cast<int>(defaultValue));
    if (value < 0) {
        throw std::invalid_argument("Argument '" + key + "' cannot be negative");
    }
    return static_cast<size_t>(value);
}

void BaseCommand::validateAllowedArguments(const std::vector<std::string>& allowed) const {
    for (const auto& [key, value] : arguments) {
        if (std::find(allowed.begin(), allowed.end(), key) == allowed.end()) {
            throw std::invalid_argument("Unknown argument: " + key);
        }
    }
}

void BaseCommand::validateRequiredArgument(const std::string& key) const {
    if (!hasArgument(key)) {
        throw std::invalid_argument("Required argument missing: " + key);
    }
}

