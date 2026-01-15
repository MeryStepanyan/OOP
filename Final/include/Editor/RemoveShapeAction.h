#pragma once
#include "IAction.h"
#include "StyledShape.h"
#include <memory>
#include "Presentation.h"

class RemoveShapeAction : public IAction {
private:
    size_t slideIndex;
    size_t shapeIndex;
    StyledShape removedShape;      // ← CHANGED: was shared_ptr<IShape>
    bool hasRemovedShape;          // ← NEW: track if we have saved a shape

public:
    RemoveShapeAction(size_t slide, size_t shape);

    void doAction(Presentation& presentation) override;
    void undoAction(Presentation& presentation) override;
};