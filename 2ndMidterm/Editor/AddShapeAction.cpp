#include "AddShapeAction.h"
#include "Presentation.h"
#include "Slide.h"
#include "IShape.h"
#include <iostream>

AddShapeAction::AddShapeAction(size_t slide, std::shared_ptr<IShape> s)
    : slideIndex(slide), shape(s), shapeIndex(0) {
    // Constructor - shapeIndex-ը կգտնվի doAction()-ում
}

void AddShapeAction::doAction(Presentation& presentation) {
    // Քայլ 1: Ստանալ slide-ը
    auto slide = presentation.getSlide(slideIndex);

    // Քայլ 2: Ավելացնել shape-ը
    slide->addShape(shape);

    // Քայլ 3: ՊԱՀԵԼ shape-ի ինդեքսը (undo-ի համար)
    // Shape-ը ավելացվել է վերջում, այնպես որ:
    shapeIndex = slide->getShapeCount() - 1;

    // Քայլ 4: Log արել
    std::cout << "Action: Added shape to slide " << slideIndex << std::endl;
}

void AddShapeAction::undoAction(Presentation& presentation) {
    // Քայլ 1: Ստանալ slide-ը
    auto slide = presentation.getSlide(slideIndex);

    // Քայլ 2: Հեռացնել shape-ը (պահված ինդեքսից)
    slide->removeShape(shapeIndex);

    // Քայլ 3: Log արել
    std::cout << "Action: Removed shape from slide " << slideIndex
        << " (undo)" << std::endl;
}

//std::string AddShapeAction::getDescription() const {
//    return "Add shape to slide " + std::to_string(slideIndex);
//}