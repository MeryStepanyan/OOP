
#include "StyledShape.h"


    StyledShape::StyledShape(std::shared_ptr<IShape> s, const ShapeStyle& st)
        : shape(s), style(st) {
    }

    std::shared_ptr<IShape> StyledShape::getShape() const { return shape; }

    const ShapeStyle& StyledShape::getStyle() const { return style; }
    void StyledShape::setShape(std::shared_ptr<IShape> s) { shape = s; }

    void StyledShape::setStyle(const ShapeStyle& st) { style = st; }

  

    void StyledShape::accept(IShapeVisitor& visitor) const {
        if (shape) {
            shape->accept(visitor);
        }
    }


    IShape::ShapeType StyledShape::getType() const {
        return shape ? shape->getType() : IShape::ShapeType::Circle;
    }


    BoundingBox StyledShape::StyledShape::getBoundingBox() const {
        return shape ? shape->getBoundingBox() : BoundingBox();
    }

    Point StyledShape::getPosition() const {
        return shape ? shape->getPosition() : Point();
    }


    void StyledShape::setPosition(float x, float y) {
        if (shape) {
            shape->setPosition(x, y);
        }
    }


    bool StyledShape::isValid() const {
        return shape != nullptr && style.isValid();
    }


    StyledShape StyledShape::withStyle(const ShapeStyle& newStyle) const {
        return StyledShape(shape, newStyle);
    }


    StyledShape StyledShape::withPen(std::shared_ptr<IPen> newPen) const {
        ShapeStyle newStyle = style;
        newStyle.setPen(newPen);
        return StyledShape(shape, newStyle);
    }


    StyledShape StyledShape::withBrush(std::shared_ptr<IBrush> newBrush) const {
        ShapeStyle newStyle = style;
        newStyle.setBrush(newBrush);
        return StyledShape(shape, newStyle);
    }
