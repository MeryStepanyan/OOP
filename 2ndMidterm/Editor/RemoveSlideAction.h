#pragma once
#include "IAction.h"
#include <memory>

class Slide;

class RemoveSlideAction : public IAction {
private:
    size_t position;                      
    std::shared_ptr<Slide> removedSlide;  // Պահված slide-ը (undo-ի համար)

public:

    explicit RemoveSlideAction(size_t pos);

    void doAction(Presentation& presentation) override;
    void undoAction(Presentation& presentation) override;
   // std::string getDescription() const override;
};