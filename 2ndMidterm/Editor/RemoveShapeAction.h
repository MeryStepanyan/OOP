#pragma once
#include "IAction.h"
#include <memory>
#include "Presentation.h"

class RemoveShapeAction : public IAction {
private:
    size_t slideIndex;                    
    size_t shapeIndex;                    
    std::shared_ptr<IShape> removedShape; 

public:
   
    RemoveShapeAction(size_t slide, size_t shape);

    void doAction(Presentation& presentation) override;
    void undoAction(Presentation& presentation) override;
    //std::string getDescription() const override;
};