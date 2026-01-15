#pragma once
#include "IAction.h"
#include "StyledShape.h"
#include <memory>
#include "Presentation.h"

class AddShapeAction : public IAction {
private:
    size_t slideIndex;              
    StyledShape styledShape; 
    size_t shapeIndex;              

public:
   
    AddShapeAction(size_t slide, const StyledShape& shape);

    void doAction(Presentation& presentation) override;
    void undoAction(Presentation& presentation) override;
    //std::string getDescription() const override;
};