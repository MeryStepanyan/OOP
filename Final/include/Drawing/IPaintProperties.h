#pragma once
#include <string>

// Paint properties for brushes and pens
class IPaintProperties {
public:
    virtual ~IPaintProperties() = default;

    // Color properties
    virtual std::string getColor() const = 0;
    virtual void setColor(const std::string& color) = 0;

    // Opacity (0.0 to 1.0)
    virtual float getOpacity() const = 0;
    virtual void setOpacity(float opacity) = 0;

    // Style properties
    virtual bool isVisible() const = 0;
    virtual void setVisible(bool visible) = 0;
};