#pragma once
#include "Presentation.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"
#include "ICommand.h"
#include <memory>
#include <string>
#include <vector>

namespace cli {
    class Controller {
    public:
        Controller();
        ~Controller() = default;

        void run();
        void exit();
        void handleError(const std::string& message);

    private:
        Presentation presentation;
        SemanticAnalyzer analyzer;
        Parser parser;
        bool m_exit;
        

        std::vector<std::unique_ptr<ICommand>> commandHistory;
        
        void executeCommand(std::unique_ptr<ICommand> command);
        void undo();
        bool canUndo() const;
        void processCommandLine(const std::string& line);
    };
}

