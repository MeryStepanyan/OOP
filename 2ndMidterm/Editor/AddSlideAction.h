#pragma once
#include "IAction.h"
#include "Presentation.h"
#include <memory>

class Slide;

class AddSlideAction : public IAction {
private:
    std::shared_ptr<Slide> slide;  
    size_t position;               

public:
   
    AddSlideAction(std::shared_ptr<Slide> s, size_t pos);

    void doAction(Presentation& presentation) override;
    void undoAction(Presentation& presentation) override;
   // std::string getDescription() const override;
};



