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
    Painter()
        : inBatch(false) {
        defaultPen = std::make_shared<Pen>("#000000", 2.0f);
        defaultBrush = std::make_shared<SolidBrush>("#FFFFFF", 1.0f);
    }

    explicit Painter(std::shared_ptr<ICanvas> c)
        : canvas(c), inBatch(false) {
        defaultPen = std::make_shared<Pen>("#000000", 2.0f);
        defaultBrush = std::make_shared<SolidBrush>("#FFFFFF", 1.0f);
    }

    // Set/Get canvas
    void setCanvas(std::shared_ptr<ICanvas> c) override {
        canvas = c;
    }

    std::shared_ptr<ICanvas> getCanvas() const override {
        return canvas;
    }

    // Draw single shape
    void drawShape(const IShape& shape) override {
        if (!canvas) {
            throw std::runtime_error("Canvas not set");
        }

        canvas->setPen(defaultPen);
        canvas->setBrush(defaultBrush);

        renderShape(shape);
    }

    void drawShape(const std::shared_ptr<IShape>& shape) override {
        if (shape) {
            drawShape(*shape);
        }
    }

    // Draw multiple shapes
    void drawShapes(const std::vector<std::shared_ptr<IShape>>& shapes) override {
        for (const auto& shape : shapes) {
            if (shape) {
                drawShape(*shape);
            }
        }
    }

    // Draw with explicit properties
    void drawShapeWithBrush(const IShape& shape, std::shared_ptr<IBrush> brush) override {
        if (!canvas) {
            throw std::runtime_error("Canvas not set");
        }

        canvas->setPen(defaultPen);
        canvas->setBrush(brush);
        renderShape(shape);
    }

    void drawShapeWithPen(const IShape& shape, std::shared_ptr<IPen> pen) override {
        if (!canvas) {
            throw std::runtime_error("Canvas not set");
        }

        canvas->setPen(pen);
        canvas->setBrush(defaultBrush);
        renderShape(shape);
    }

    void drawShapeWithProperties(const IShape& shape,
        std::shared_ptr<IBrush> brush,
        std::shared_ptr<IPen> pen) override {
        if (!canvas) {
            throw std::runtime_error("Canvas not set");
        }

        canvas->setPen(pen);
        canvas->setBrush(brush);
        renderShape(shape);
    }

    // Batch operations
    void beginBatch() override {
        inBatch = true;
    }

    void endBatch() override {
        inBatch = false;
    }

    // Clear operations
    void clearCanvas() override {
        if (canvas) {
            canvas->clear();
        }
    }

    void clearCanvas(const std::string& backgroundColor) override {
        if (canvas) {
            canvas->clear(backgroundColor);
        }
    }

private:
    void renderShape(const IShape& shape) {
        switch (shape.getType()) {
        case IShape::ShapeType::Circle: {
            const Circle* circle = dynamic_cast<const Circle*>(&shape);
            if (circle) {
                Point pos = circle->getPosition();
                canvas->drawCircle(pos.x, pos.y, circle->getRadius());
            }
            break;
        }

        case IShape::ShapeType::Rectangle: {
            const Rectangle* rect = dynamic_cast<const Rectangle*>(&shape);
            if (rect) {
                Point pos = rect->getPosition();
                canvas->drawRectangle(pos.x, pos.y, rect->getWidth(), rect->getHeight());
            }
            break;
        }

        case IShape::ShapeType::Text: {
            const Text* text = dynamic_cast<const Text*>(&shape);
            if (text) {
                Point pos = text->getPosition();
                canvas->drawText(pos.x, pos.y, text->getContent(), text->getFontSize());
            }
            break;
        }

        case IShape::ShapeType::Image: {
            const Image* image = dynamic_cast<const Image*>(&shape);
            if (image) {
                // For images, we'd need to add drawImage to ICanvas
                // For now, draw a placeholder rectangle
                Point pos = image->getPosition();
                canvas->drawRectangle(pos.x, pos.y, image->getWidth(), image->getHeight());
            }
            break;
        }
        }
    }
};