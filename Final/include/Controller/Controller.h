#pragma once
#include "Editor.h"
#include "Presentation.h"
#include <memory>

class SemanticAnalyzer;
class ICommand;
class Parser;
class Controller {
public:
    Controller();
    ~Controller();
    void run();
    void exit();

private:
    bool m_exit;

    // Order matters! Presentation must be created before Editor
    Presentation presentation;                          // Created first
    std::unique_ptr<SemanticAnalyzer> semanticAnalyzer; // Created second
    std::unique_ptr<Editor> editor;                     // Needs presentation reference
    std::unique_ptr<Parser> parser;                     // Needs semanticAnalyzer pointer
};