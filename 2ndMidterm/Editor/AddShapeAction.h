#pragma once
#include "IAction.h"
#include <memory>
#include "Presentation.h"

class AddShapeAction : public IAction {
private:
    size_t slideIndex;              // Slide-ի ինդեքս
    std::shared_ptr<IShape> shape;  // Ավելացվող shape-ը
    size_t shapeIndex;              // Որտեղ ավելացվեց (undo-ի համար)

public:
   
    AddShapeAction(size_t slide, std::shared_ptr<IShape> s);

    void doAction(Presentation& presentation) override;
    void undoAction(Presentation& presentation) override;
    //std::string getDescription() const override;
};