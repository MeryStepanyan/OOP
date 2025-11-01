//CommandFactory.cpp
#include "CommandFactory.h"
#include <stdexcept>
#include <algorithm>

std::map<std::string, CommandFactory::CreatorFunc>& CommandFactory::getRegistry() {
    static std::map<std::string, CreatorFunc> registry;
    return registry;
}

void CommandFactory::registerCommand(const std::string& name, CreatorFunc creator) {
    getRegistry()[name] = creator;
}

std::unique_ptr<ICommand> CommandFactory::create(
    const std::string& cmdName,
    const std::unordered_map<std::string, std::string>& args)
{
    auto& registry = getRegistry();
    auto it = registry.find(cmdName);
    
    if (it != registry.end()) {
        return it->second(args);
    }
    
    throw std::invalid_argument("Unknown command: " + cmdName);
}

std::vector<std::string> CommandFactory::getRegisteredCommands() {
    std::vector<std::string> commands;
    auto& registry = getRegistry();
    
    for (const auto& [name, creator] : registry) {
        commands.push_back(name);
    }
    
    return commands;
}

bool CommandFactory::isRegistered(const std::string& cmdName) {
    auto& registry = getRegistry();
    return registry.find(cmdName) != registry.end();
}

// Note: Command registration is now done separately via CommandRegistryInitializer
// This keeps CommandFactory decoupled from concrete command implementations
