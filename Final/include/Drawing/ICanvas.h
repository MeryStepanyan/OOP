#pragma once
#include "Geometry.h"
#include <memory>
#include <vector>
#include <string>

class IPen;
class IBrush;

// Canvas interface for drawing operations
class ICanvas {
public:
    virtual ~ICanvas() = default;

    // Canvas dimensions
    virtual float getWidth() const = 0;
    virtual float getHeight() const = 0;
    virtual void setSize(float width, float height) = 0;

    // Clear canvas
    virtual void clear() = 0;
    virtual void clear(const std::string& backgroundColor) = 0;

    // Drawing primitives
    virtual void drawLine(float x1, float y1, float x2, float y2) = 0;
    virtual void drawRectangle(float x, float y, float width, float height) = 0;
    virtual void drawCircle(float centerX, float centerY, float radius) = 0;
    virtual void drawEllipse(float centerX, float centerY, float radiusX, float radiusY) = 0;
    virtual void drawText(float x, float y, const std::string& text, float fontSize) = 0;
    virtual void drawPolygon(const std::vector<Point>& points) = 0;

    // Path operations
    virtual void beginPath() = 0;
    virtual void moveTo(float x, float y) = 0;
    virtual void lineTo(float x, float y) = 0;
    virtual void arcTo(float x1, float y1, float x2, float y2, float radius) = 0;
    virtual void closePath() = 0;
    virtual void stroke() = 0;
    virtual void fill() = 0;

    // Transform operations
    virtual void save() = 0;  // Save current state
    virtual void restore() = 0;  // Restore saved state
    virtual void translate(float dx, float dy) = 0;
    virtual void rotate(float angle) = 0;
    virtual void scale(float sx, float sy) = 0;

    // Set drawing properties
    virtual void setPen(std::shared_ptr<IPen> pen) = 0;
    virtual void setBrush(std::shared_ptr<IBrush> brush) = 0;
    virtual std::shared_ptr<IPen> getPen() const = 0;
    virtual std::shared_ptr<IBrush> getBrush() const = 0;

    // Clipping
    virtual void setClipRect(float x, float y, float width, float height) = 0;
    virtual void clearClip() = 0;

    // Export/Save
    virtual bool saveToFile(const std::string& filename) const = 0;
    virtual std::string exportToSVG() const = 0;
};
