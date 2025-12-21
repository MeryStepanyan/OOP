// Editor.h
#pragma once
#include "Presentation.h"
#include "IAction.h"
#include <stack>
#include <memory>

class Editor {
public:
    explicit Editor(Presentation& pres);

    void doAction(std::unique_ptr<IAction> action);
    void undo();
    void redo();

    Presentation& getPresentation();
private:
    Presentation& presentation;
    std::stack<std::unique_ptr<IAction>> undoStack;
    std::stack<std::unique_ptr<IAction>> redoStack;


};