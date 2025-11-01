//CommandExecutor.cpp
#include "CommandExecutor.h"
#include <iostream>
#include <algorithm>

CommandExecutor::CommandExecutor(ISlideRepository& repository)
    : repository(repository) {
}

void CommandExecutor::executeCommand(std::unique_ptr<ICommand> command) {
    if (!command) {
        throw std::invalid_argument("Cannot execute null command");
    }
    
    try {
        // Execute the command
        handleCommandExecution(*command);
        
        // Only store commands that support undo (after successful execution)
        if (command->canUndo()) {
            commandHistory.push_back(std::move(command));
        }
        // Otherwise, command will be destroyed after execution
    }
    catch (const std::exception& e) {
        // Command failed - don't store it in history
        std::cerr << "Error executing command: " << e.what() << std::endl;
        throw; // Re-throw to allow caller to handle if needed
    }
}

void CommandExecutor::undo() {
    if (commandHistory.empty()) {
        throw std::runtime_error("No commands to undo");
    }
    
    auto& lastCommand = commandHistory.back();
    if (!lastCommand->canUndo()) {
        throw std::runtime_error("Last command does not support undo");
    }
    
    try {
        lastCommand->undo(repository);
        commandHistory.pop_back();
        std::cout << "Command undone: " << lastCommand->getName() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error undoing command: " << e.what() << std::endl;
        throw;
    }
}

bool CommandExecutor::canUndo() const {
    return !commandHistory.empty() && 
           commandHistory.back()->canUndo();
}

void CommandExecutor::clearHistory() {
    commandHistory.clear();
}

size_t CommandExecutor::getHistorySize() const {
    return commandHistory.size();
}

void CommandExecutor::handleCommandExecution(ICommand& command) {
    command.execute(repository);
}

