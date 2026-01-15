#include "RemoveShapeAction.h"
#include "Presentation.h"
#include "Slide.h"
#include <iostream>

RemoveShapeAction::RemoveShapeAction(size_t slide, size_t shape)
    : slideIndex(slide),
    shapeIndex(shape),
    removedShape(nullptr, ShapeStyle()),  // Initialize with dummy values
    hasRemovedShape(false) {
}

void RemoveShapeAction::doAction(Presentation& presentation) {
    auto slide = presentation.getSlide(slideIndex);

    // Save the shape before removing (for undo)
    if (shapeIndex < slide->getShapeCount()) {
        removedShape = slide->getShape(shapeIndex);  
        hasRemovedShape = true;
    }

    slide->removeShape(shapeIndex);

    std::cout << "Action: Removed shape from slide " << slideIndex
        << " at index " << shapeIndex << std::endl;
}

void RemoveShapeAction::undoAction(Presentation& presentation) {
    if (!hasRemovedShape) {
        return;
    }

    auto slide = presentation.getSlide(slideIndex);
    slide->addShape(removedShape);  // ← Now accepts StyledShape

    std::cout << "Action: Restored shape to slide " << slideIndex
        << " (undo)" << std::endl;
}