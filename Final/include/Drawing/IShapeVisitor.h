#pragma once

// Forward declarations to avoid circular dependencies
class Circle;
class Rectangle;
class Text;
class Image;
class Line;
class Triangle;
class Ellipse;
class Polygon;
class Square;


 
class IShapeVisitor {
public:
    virtual ~IShapeVisitor() = default;

   
    virtual void visit(const Circle& circle) = 0;
    virtual void visit(const Rectangle& rect) = 0;
    virtual void visit(const Text& text) = 0;
    virtual void visit(const Image& image) = 0;
    virtual void visit(const Line& line) = 0;
    virtual void visit(const Triangle& triangle) = 0;
    virtual void visit(const Ellipse& ellipse) = 0;
    virtual void visit(const Polygon& plygon) = 0;
    virtual void visit(const Square& square) = 0;
};