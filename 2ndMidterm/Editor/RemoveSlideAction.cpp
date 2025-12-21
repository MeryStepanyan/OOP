
#include "RemoveSlideAction.h"
#include "Presentation.h"
#include "Slide.h"
#include "IShape.h"
#include <iostream>

RemoveSlideAction::RemoveSlideAction(size_t pos)
    : position(pos), removedSlide(nullptr) {
 
}

void RemoveSlideAction::doAction(Presentation& presentation) {
    
    removedSlide = presentation.getSlide(position);
    presentation.removeSlide(position);

    std::cout << "Action: Removed slide at position " << position << std::endl;
}

void RemoveSlideAction::undoAction(Presentation& presentation) {
  
    presentation.addSlide(position, removedSlide);

    std::cout << "Action: Restored slide at position " << position
        << " (undo)" << std::endl;
}

//std::string RemoveSlideAction::getDescription() const {
//    return "Remove slide at position " + std::to_string(position);
//}