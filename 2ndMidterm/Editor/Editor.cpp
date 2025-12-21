// Editor.cpp
#include "Editor.h"
#include <iostream>

Editor::Editor(Presentation& pres)
    : presentation(pres) {
}

void Editor::doAction(std::unique_ptr<IAction> action) {
    action->doAction(presentation);
    
    undoStack.push(std::move(action));
    while(!redoStack.empty()) redoStack.pop();
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

Presentation& Editor::getPresentation() { return presentation; }
