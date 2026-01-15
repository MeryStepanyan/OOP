#pragma once
#include "BaseCommand.h"
#include "Editor.h"
#include <iostream>


class UndoCommand : public BaseCommand {
public:
    UndoCommand() = default;
    explicit UndoCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    std::string getName() const { return "Undo"; }

    void execute(Editor& editor) {
        editor.undo();
        std::cout << "Undo completed" << std::endl;
    }
};


class RedoCommand : public BaseCommand {
public:
    RedoCommand() = default;
    explicit RedoCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    std::string getName() const { return "Redo"; }

    void execute(Editor& editor) {
        editor.redo();
        std::cout << "Redo completed" << std::endl;
    }
};