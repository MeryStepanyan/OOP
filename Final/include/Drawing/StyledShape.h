#pragma once
#include "IShape.h"
#include "ShapeStyle.h"
#include "IShapeVisitor.h"
#include <memory>


class StyledShape {
private:
    std::shared_ptr<IShape> shape;  // The geometry
    ShapeStyle style;               // The appearance

public:

    StyledShape(std::shared_ptr<IShape> s, const ShapeStyle& st);

    std::shared_ptr<IShape> getShape() const;

    const ShapeStyle& getStyle() const;
    void setShape(std::shared_ptr<IShape> s);

    void setStyle(const ShapeStyle& st);

    void accept(IShapeVisitor& visitor) const;
  
    IShape::ShapeType getType() const;

    BoundingBox getBoundingBox() const;

    Point getPosition() const;
 
    void setPosition(float x, float y);


    bool isValid() const;
   
    StyledShape withStyle(const ShapeStyle& newStyle) const;

   
    StyledShape withPen(std::shared_ptr<IPen> newPen) const;

    
    StyledShape withBrush(std::shared_ptr<IBrush> newBrush) const;
};