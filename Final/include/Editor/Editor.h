// Editor.h
#pragma once
#include "IAction.h"
#include <stack>
#include <memory>
#include <string>

class Presentation;

class Editor {
public:
    explicit Editor(Presentation& pres);

    void doAction(std::unique_ptr<IAction> action);
    void undo();
    void redo();

    Presentation& getPresentation();

    // File tracking methods
    void setCurrentFile(const std::string& filename);
    std::string getCurrentFile() const;
    bool hasCurrentFile() const;
    void clearCurrentFile();

    // Auto-save to current file
    bool saveToCurrentFile();

private:
    Presentation& presentation;
    std::stack<std::unique_ptr<IAction>> undoStack;
    std::stack<std::unique_ptr<IAction>> redoStack;
    std::string currentFile;  // Track currently opened file
};
