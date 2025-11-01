#include "Controller.h"
#include "ISlideRepository.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace cli {
    Controller::Controller()
        : presentation(),
          analyzer(),
          parser(&analyzer),
          m_exit(false) {
    }

    void Controller::run() {
        std::string line;
        std::cout << "Enter command (e.g., addslide -pos 5, or 'undo' to undo last command): ";

        while (!m_exit && std::getline(std::cin, line)) {
            if (line.empty()) {
                std::cout << "Enter command: ";
                continue;
            }

            processCommandLine(line);

            if (!m_exit) {
                std::cout << "\nEnter command: ";
            }
        }

        std::cout << "Goodbye!\n";
    }

    void Controller::exit() {
        m_exit = true;
    }

    void Controller::handleError(const std::string& message) {
        std::cerr << "Error: " << message << std::endl;
    }

    void Controller::processCommandLine(const std::string& line) {
        // Check for exit commands
        if (line == "exit" || line == "quit") {
            exit();
            return;
        }

        // Handle undo command
        if (line == "undo" || line == "u") {
            try {
                undo();
            }
            catch (const std::exception& e) {
                handleError(e.what());
            }
            return;
        }

        // Parse and execute command
        std::istringstream input(line);
        std::unique_ptr<ICommand> cmd = parser.parseCommand(input);

        if (cmd != nullptr) {
            try {
                executeCommand(std::move(cmd));
            }
            catch (const std::exception& e) {
                handleError(e.what());
            }
        }
        else {
            handleError("Failed to parse command");
        }
    }

    void Controller::executeCommand(std::unique_ptr<ICommand> command) {
        if (!command) {
            throw std::invalid_argument("Cannot execute null command");
        }

        try {
            // Execute the command
            ISlideRepository& repository = presentation;
            command->execute(repository);

            // Only store commands that support undo (after successful execution)
            if (command->canUndo()) {
                commandHistory.push_back(std::move(command));
            }
            // Otherwise, command will be destroyed after execution
        }
        catch (const std::exception& e) {
            // Command failed - don't store it in history
            throw; // Re-throw to allow caller to handle
        }
    }

    void Controller::undo() {
        if (commandHistory.empty()) {
            throw std::runtime_error("No commands to undo");
        }

        auto& lastCommand = commandHistory.back();
        if (!lastCommand->canUndo()) {
            throw std::runtime_error("Last command does not support undo");
        }

        try {
            ISlideRepository& repository = presentation;
            lastCommand->undo(repository);
            commandHistory.pop_back();
            std::cout << "Command undone: " << lastCommand->getName() << std::endl;
        }
        catch (const std::exception& e) {
            throw;
        }
    }

    bool Controller::canUndo() const {
        return !commandHistory.empty() &&
            commandHistory.back()->canUndo();
    }
}

