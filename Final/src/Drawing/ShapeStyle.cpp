#include "ShapeStyle.h"

    ShapeStyle::ShapeStyle()
        : pen(nullptr), brush(nullptr), name("default") {
    }


    ShapeStyle::ShapeStyle(std::shared_ptr<IPen> p,
        std::shared_ptr<IBrush> b,
        const std::string& n )
        : pen(p), brush(b), name(n) {
    }

    // Getters
    std::shared_ptr<IPen> ShapeStyle::getPen() const { return pen; }
    std::shared_ptr<IBrush> ShapeStyle::getBrush() const { return brush; }
    std::string ShapeStyle::getName() const { return name; }

    // Setters
    void ShapeStyle::setPen(std::shared_ptr<IPen> p) { pen = p; }
    void ShapeStyle::setBrush(std::shared_ptr<IBrush> b) { brush = b; }
    void ShapeStyle::setName(const std::string& n) { name = n; }


    ShapeStyle ShapeStyle::clone() const {
        return ShapeStyle(pen, brush, name);
    }


    bool ShapeStyle::hasPen() const { return pen != nullptr; }


    bool ShapeStyle::hasBrush() const { return brush != nullptr; }

    bool ShapeStyle::isValid() const { return hasPen() || hasBrush(); }
