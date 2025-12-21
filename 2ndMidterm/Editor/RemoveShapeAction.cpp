#include "RemoveShapeAction.h"
#include "Presentation.h"
#include "Slide.h"
#include "IShape.h"
#include <iostream>


RemoveShapeAction::RemoveShapeAction(size_t slide, size_t shape)
    : slideIndex(slide), shapeIndex(shape), removedShape(nullptr) {
    // Constructor - removedShape-ը դեռ nullptr է
    // Այն կպահվի doAction()-ում
}

void RemoveShapeAction::doAction(Presentation& presentation) {
    auto slide = presentation.getSlide(slideIndex);

    // Քայլ 2: ՊԱՀԵԼ shape-ը ՄԻՆՉ հեռացնելը (undo-ի համար)
    const auto& shapes = slide->getShapes();
    if (shapeIndex < shapes.size()) {
        removedShape = shapes[shapeIndex];
    }

    slide->removeShape(shapeIndex);

    std::cout << "Action: Removed shape from slide " << slideIndex
        << " at index " << shapeIndex << std::endl;
}

void RemoveShapeAction::undoAction(Presentation& presentation) {

    auto slide = presentation.getSlide(slideIndex);

    slide->addShape(removedShape);

 
    std::cout << "Action: Restored shape to slide " << slideIndex
        << " (undo)" << std::endl;
}

//std::string RemoveShapeAction::getDescription() const {
//    return "Remove shape from slide " + std::to_string(slideIndex);
//}