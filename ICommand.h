
#pragma once
#include <memory>
#include <string>


class ISlideRepository;

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(ISlideRepository& repository) = 0;
    virtual bool canUndo() const { return false; }
    virtual void undo(ISlideRepository& repository) {
        // empty for commands that don't support undo
    }
    
  
    virtual std::string getName() const = 0;
};
