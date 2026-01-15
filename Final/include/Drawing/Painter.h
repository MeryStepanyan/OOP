#pragma once
#include "IPainter.h"
#include "Pen.h"
#include "SolidBrush.h"
#include <stdexcept>

class Painter : public IPainter {
private:
    std::shared_ptr<ICanvas> canvas;
    std::shared_ptr<IPen> defaultPen;
    std::shared_ptr<IBrush> defaultBrush;
    bool inBatch;

public:
    Painter();
    explicit Painter(std::shared_ptr<ICanvas> c);

    // Set/Get canvas
    void setCanvas(std::shared_ptr<ICanvas> c) override;

    std::shared_ptr<ICanvas> getCanvas() const override;

    // Draw single shape
    void drawShape(const IShape& shape) override;

    void drawShape(const std::shared_ptr<IShape>& shape) override;

    // Draw multiple shapes
    void drawShapes(const std::vector<std::shared_ptr<IShape>>& shapes) override;

    // Draw with explicit properties
    void drawShapeWithBrush(const IShape& shape, std::shared_ptr<IBrush> brush) override;

    void drawShapeWithPen(const IShape& shape, std::shared_ptr<IPen> pen) override;

    void drawShapeWithProperties(const IShape& shape,
        std::shared_ptr<IBrush> brush,
        std::shared_ptr<IPen> pen) override;

    // Batch operations
    void beginBatch() override;

    void endBatch() override;

    // Clear operations
    void clearCanvas() override;

    void clearCanvas(const std::string& backgroundColor) override;

private:
    void renderShape(const IShape& shape);
};