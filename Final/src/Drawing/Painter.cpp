
#include "Painter.h"



Painter::Painter()
        : inBatch(false) {
        defaultPen = std::make_shared<Pen>("#000000", 2.0f);
        defaultBrush = std::make_shared<SolidBrush>("#FFFFFF", 1.0f);
    }

Painter::Painter(std::shared_ptr<ICanvas> c)
        : canvas(c), inBatch(false) {
        defaultPen = std::make_shared<Pen>("#000000", 2.0f);
        defaultBrush = std::make_shared<SolidBrush>("#FFFFFF", 1.0f);
    }

    // Set/Get canvas
    void Painter::setCanvas(std::shared_ptr<ICanvas> c)  {
        canvas = c;
    }

    std::shared_ptr<ICanvas> Painter::getCanvas() const  {
        return canvas;
    }

    // Draw single shape
    void Painter::drawShape(const IShape& shape)  {
        if (!canvas) {
            throw std::runtime_error("Canvas not set");
        }

        canvas->setPen(defaultPen);
        canvas->setBrush(defaultBrush);

        renderShape(shape);
    }

    void Painter::drawShape(const std::shared_ptr<IShape>& shape)  {
        if (shape) {
            drawShape(*shape);
        }
    }

    // Draw multiple shapes
    void Painter::drawShapes(const std::vector<std::shared_ptr<IShape>>& shapes) {
        for (const auto& shape : shapes) {
            if (shape) {
                drawShape(*shape);
            }
        }
    }

    // Draw with explicit properties
    void Painter::drawShapeWithBrush(const IShape& shape, std::shared_ptr<IBrush> brush)  {
        if (!canvas) {
            throw std::runtime_error("Canvas not set");
        }

        canvas->setPen(defaultPen);
        canvas->setBrush(brush);
        renderShape(shape);
    }

    void Painter::drawShapeWithPen(const IShape& shape, std::shared_ptr<IPen> pen)  {
        if (!canvas) {
            throw std::runtime_error("Canvas not set");
        }

        canvas->setPen(pen);
        canvas->setBrush(defaultBrush);
        renderShape(shape);
    }

    void Painter::drawShapeWithProperties(const IShape& shape,
        std::shared_ptr<IBrush> brush,
        std::shared_ptr<IPen> pen)  {
        if (!canvas) {
            throw std::runtime_error("Canvas not set");
        }

        canvas->setPen(pen);
        canvas->setBrush(brush);
        renderShape(shape);
    }

    // Batch operations
    void Painter::beginBatch()  {
        inBatch = true;
    }

    void Painter::endBatch()  {
        inBatch = false;
    }

    // Clear operations
    void Painter::clearCanvas()  {
        if (canvas) {
            canvas->clear();
        }
    }

    void Painter::clearCanvas(const std::string& backgroundColor)  {
        if (canvas) {
            canvas->clear(backgroundColor);
        }
    }


    void Painter::renderShape(const IShape& shape) {
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
