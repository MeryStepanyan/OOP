//#pragma once
//#include "IShape.h"
//
//class UShape : public IShape {
//protected:
//    Point m_pos;
//    Size m_size;
//
//public:
//    UShape(Point pos, Size size) : m_pos(pos), m_size(size) {}
//    virtual ~UShape() = default;
//
//    void setPosition(Point pos) override { m_pos = pos; }
//    Point position() const override { return m_pos; }
//
//    void setSize(Size s) override { m_size = s; }
//    Size size() const override { return m_size; }
//
//    Rect boundingBox() const override { return { m_pos, m_size }; }
//};
//
//
//
//class RectangleShape : public UShape {
//    Rect rect;
//
//public:
//    RectangleShape(Point pos, Size size) : UShape(pos, size) {}
//
//    void draw() const override {
//        std::cout << "Drawing Rectangle at ("
//            << rect.pos.x << ", " << rect.pos.y
//            << ") size [" << rect.size.width << " x " << rect.size.height << "]\n";
//    }
//};
//
//
//
//class EllipseShape : public UShape {
//    Rect rect;
//
//public:
//    EllipseShape(Point pos, Size size) : UShape(pos, size) {}
//
//    void draw() const override {
//        std::cout << "Drawing Ellipse at ("
//            << rect.pos.x << ", " << rect.pos.y
//            << ") size [" << rect.size.width << " x " << rect.size.height << "]\n";
//    }
//};
//
