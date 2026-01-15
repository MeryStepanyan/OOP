#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <map>
#include "ICommand.h"

/**
 * CommandFactory - Creates command objects using Factory Pattern
 *
 * Now includes registration logic (merged from CommandRegistry)
 * All commands are registered in CommandFactory.cpp
 */
class CommandFactory {
public:
    using CreatorFunc = std::function<std::unique_ptr<ICommand>(
        const std::unordered_map<std::string, std::string>&)>;

    // Register a new command
    static void registerCommand(const std::string& name, CreatorFunc creator);

    // Create command by name
    static std::unique_ptr<ICommand> create(
        const std::string& cmdName,
        const std::unordered_map<std::string, std::string>& args);

    // Get list of registered commands
    static std::vector<std::string> getRegisteredCommands();

    // Check if command exists
    static bool isRegistered(const std::string& cmdName);

private:
    // Static registry map
    static std::map<std::string, CreatorFunc>& getRegistry();

    // Initialize all commands (called automatically)
    static void initializeRegistry();

    // Delete constructors (static-only class)
    CommandFactory() = delete;
    CommandFactory(const CommandFactory&) = delete;
    CommandFactory& operator=(const CommandFactory&) = delete;
};