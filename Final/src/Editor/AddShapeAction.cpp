#include "AddShapeAction.h"
#include "Presentation.h"
#include "Slide.h"
#include <iostream>

AddShapeAction::AddShapeAction(size_t slide, const StyledShape& shape)
    : slideIndex(slide), styledShape(shape), shapeIndex(0) {
}

void AddShapeAction::doAction(Presentation& presentation) {
    auto slide = presentation.getSlide(slideIndex);
    slide->addShape(styledShape);
    shapeIndex = slide->getShapeCount() - 1;

    std::cout << "Action: Added styled shape to slide " << slideIndex << std::endl;
}

void AddShapeAction::undoAction(Presentation& presentation) {
    auto slide = presentation.getSlide(slideIndex);
    slide->removeShape(shapeIndex);

    std::cout << "Action: Removed shape from slide " << slideIndex
        << " (undo)" << std::endl;
}