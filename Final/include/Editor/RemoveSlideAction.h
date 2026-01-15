#pragma once
#include "IAction.h"
#include <memory>

class Slide;

class RemoveSlideAction : public IAction {
private:
    size_t position;                      // Որտեղից հեռացնել
    std::shared_ptr<Slide> removedSlide;  // Պահված slide-ը (undo-ի համար)

public:
    /**
     * Constructor
     * @param pos - հեռացվող slide-ի դիրքը
     */
    explicit RemoveSlideAction(size_t pos);

    void doAction(Presentation& presentation) override;
    void undoAction(Presentation& presentation) override;
   // std::string getDescription() const override;
};