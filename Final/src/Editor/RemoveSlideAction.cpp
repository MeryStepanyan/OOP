
#include "RemoveSlideAction.h"
#include "Presentation.h"
#include "Slide.h"
#include "IShape.h"
#include <iostream>

RemoveSlideAction::RemoveSlideAction(size_t pos)
    : position(pos), removedSlide(nullptr) {
    // Constructor - removedSlide-ը դեռ nullptr է
    // Այն կպահվի doAction()-ում
}

void RemoveSlideAction::doAction(Presentation& presentation) {
    // Քայլ 1: ՊԱՀԵԼ slide-ը ՄԻՆՉ հեռացնելը (undo-ի համար)
    removedSlide = presentation.getSlide(position);

    // Քայլ 2: Հեռացնել slide-ը
    presentation.removeSlide(position);

    // Քայլ 3: Log արել
    std::cout << "Action: Removed slide at position " << position << std::endl;
}

void RemoveSlideAction::undoAction(Presentation& presentation) {
    // Քայլ 1: Վերականգնել slide-ը նույն դիրքում
    presentation.addSlide(position, removedSlide);

    // Քայլ 2: Log արել
    std::cout << "Action: Restored slide at position " << position
        << " (undo)" << std::endl;
}

//std::string RemoveSlideAction::getDescription() const {
//    return "Remove slide at position " + std::to_string(position);
//}