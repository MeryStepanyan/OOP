#include "Controller.h"
#include "SemanticAnalyzer.h"
#include "Parser.h"
#include "ICommand.h"
#include <iostream>
#include <sstream>

Controller::Controller()
    : m_exit(false),
    presentation(),
    semanticAnalyzer(new SemanticAnalyzer()),
    editor(new Editor(presentation)),
    parser(new Parser(semanticAnalyzer.get()))
{
}

Controller::~Controller() {
    // Unique pointers auto-cleanup
}

void Controller::run() {
    std::string line;
    std::cout << "===========================================\n";
    std::cout << "   PRESENTATION EDITOR\n";
    std::cout << "===========================================\n";
    std::cout << "Type 'help' for available commands\n";
    std::cout << "Type 'exit' or 'quit' to exit\n\n";

    while (!m_exit) {
        std::cout << "> ";
        std::getline(std::cin, line);

        if (line.empty()) {
            continue;
        }

        try {
            std::cout << "Parsing: " << line << std::endl;

            // Create a string stream from the input line
            std::istringstream iss(line);

            // Call parseCommand with the stream
            auto command = parser->parseCommand(iss);

            if (command) {
                std::cout << "Executing command: " << command->getName() << std::endl;

                // Выполнить команду
                // Если это ExitCommand, оно вызовет std::exit(0)
                // и программа завершится здесь
                command->execute(*editor);

                std::cout << "Command executed successfully.\n" << std::endl;
            }
            else {
                std::cerr << "Error: Failed to parse command.\n" << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n" << std::endl;
        }
    }

    std::cout << "Controller exited.\n";
}

void Controller::exit() {
    m_exit = true;
}