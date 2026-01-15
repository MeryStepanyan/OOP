#include "Editor.h"
#include "Presentation.h"
#include "SaveCommand.h"
#include <iostream>

Editor::Editor(Presentation& pres)
    : presentation(pres), currentFile("") {
}

void Editor::doAction(std::unique_ptr<IAction> action) {
    action->doAction(presentation);

    undoStack.push(std::move(action));
    while (!redoStack.empty()) redoStack.pop();
}

void Editor::undo() {
    if (undoStack.empty()) {
        std::cout << "Nothing to undo\n";
        return;
    }

    auto action = std::move(undoStack.top());
    undoStack.pop();

    action->undoAction(presentation);
    redoStack.push(std::move(action));
}

void Editor::redo() {
    if (redoStack.empty()) {
        std::cout << "Nothing to redo\n";
        return;
    }

    auto action = std::move(redoStack.top());
    redoStack.pop();

    action->doAction(presentation);
    undoStack.push(std::move(action));
}

Presentation& Editor::getPresentation() {
    return presentation;
}

void Editor::setCurrentFile(const std::string& filename) {
    currentFile = filename;
    std::cout << "Current file set to: " << filename << std::endl;
}

std::string Editor::getCurrentFile() const {
    return currentFile;
}

bool Editor::hasCurrentFile() const {
    return !currentFile.empty();
}

void Editor::clearCurrentFile() {
    currentFile = "";
}

bool Editor::saveToCurrentFile() {
    if (!hasCurrentFile()) {
        std::cerr << "No current file set. Use 'save -file <filename>' first." << std::endl;
        return false;
    }

    // Reuse save logic
    std::unordered_map<std::string, std::string> args;
    args["file"] = currentFile;
    SaveCommand saveCmd(args);

    try {
        saveCmd.execute(*this);
        std::cout << "Auto-saved to: " << currentFile << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Auto-save failed: " << e.what() << std::endl;
        return false;
    }
}