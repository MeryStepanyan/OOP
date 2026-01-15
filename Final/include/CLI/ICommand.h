
#pragma once
#include <memory>
#include <string>


class Editor;

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(Editor& editor) = 0;
   // virtual bool canUndo() const { return false; }
    //virtual void undo(ISlideRepository& repository) {
        // empty for commands that don't support undo
    //}


    virtual std::string getName() const = 0;
};
