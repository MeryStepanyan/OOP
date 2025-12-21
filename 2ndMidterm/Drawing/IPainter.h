#pragma once
#include "ICanvas.h"
#include "IShape.h"
#include "IBrush.h"
#include "IPen.h"
#include <memory>
#include <vector>

// Painter interface for high-level drawing operations
class IPainter {
public:
    virtual ~IPainter() = default;

    // Set the canvas to paint on
    virtual void setCanvas(std::shared_ptr<ICanvas> canvas) = 0;
    virtual std::shared_ptr<ICanvas> getCanvas() const = 0;

    // Draw shapes
    virtual void drawShape(const IShape& shape) = 0;
    virtual void drawShape(const std::shared_ptr<IShape>& shape) = 0;

    // Draw multiple shapes
    virtual void drawShapes(const std::vector<std::shared_ptr<IShape>>& shapes) = 0;

    // Drawing operations with explicit properties
    virtual void drawShapeWithBrush(const IShape& shape, std::shared_ptr<IBrush> brush) = 0;
    virtual void drawShapeWithPen(const IShape& shape, std::shared_ptr<IPen> pen) = 0;
    virtual void drawShapeWithProperties(const IShape& shape,
        std::shared_ptr<IBrush> brush,
        std::shared_ptr<IPen> pen) = 0;

    // Batch operations
    virtual void beginBatch() = 0;
    virtual void endBatch() = 0;

    // Clear operations
    virtual void clearCanvas() = 0;
    virtual void clearCanvas(const std::string& backgroundColor) = 0;
};