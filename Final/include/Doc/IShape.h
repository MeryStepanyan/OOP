#pragma once
#include "Geometry.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <memory>
#include <vector>

// Forward declaration for Visitor pattern
class IShapeVisitor;

// Abstract base class for all shapes
class IShape {
public:
    enum class ShapeType {
        Circle,
        Rectangle,
        Text,
        Image,
        Line,        // NEW
        Triangle,    // NEW
        Ellipse,     // NEW
        Polygon,     // NEW - for custom shapes
        Square       // NEW
    };

    virtual ~IShape() = default;

    // Pure virtual functions that all shapes must implement
    virtual void draw() const = 0;
    virtual void setPosition(float x, float y) = 0;
    virtual ShapeType getType() const = 0;
    virtual void accept(IShapeVisitor& visitor) const = 0;
    virtual BoundingBox getBoundingBox() const = 0;
    virtual Point getPosition() const = 0;

protected:
    BoundingBox box;

    // Helper validation methods
    static void validatePositive(float value, const std::string& name) {
        if (value <= 0.0f || !std::isfinite(value)) {
            throw std::invalid_argument(name + " must be positive and finite");
        }
    }

    static void validateFinite(float value, const std::string& name) {
        if (!std::isfinite(value)) {
            throw std::invalid_argument(name + " must be finite");
        }
    }
};
