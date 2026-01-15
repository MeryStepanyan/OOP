#include "AddSlideAction.h"
#include "Presentation.h"
#include "Slide.h"
#include "IShape.h"
#include <iostream>


// ============================================================================
// ADD SLIDE ACTION IMPLEMENTATION
// ============================================================================

AddSlideAction::AddSlideAction(std::shared_ptr<Slide> s, size_t pos)
    : slide(s), position(pos) {
    // Constructor - պարզապես պահում է տվյալները
}

void AddSlideAction::doAction(Presentation& presentation) {
    // Քայլ 1: Ավելացնել slide-ը presentation-ին
    presentation.addSlide(position, slide);

    // Քայլ 2: Log արել (debugging-ի համար)
    std::cout << "Action: Added slide at position " << position << std::endl;
}

void AddSlideAction::undoAction(Presentation& presentation) {
    // Քայլ 1: Հեռացնել slide-ը (հակադարձ գործողություն)
    presentation.removeSlide(position);

    // Քայլ 2: Log արել
    std::cout << "Action: Removed slide at position " << position
        << " (undo)" << std::endl;
}

//std::string AddSlideAction::getDescription() const {
//    return "Add slide at position " + std::to_string(position);
//}