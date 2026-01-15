// SVGRenderer.h
#pragma once
#include "IShapeVisitor.h"
#include "ICanvas.h"
#include "IShape.h"
#include <memory>

class SVGRenderer : public IShapeVisitor {
private:
    std::shared_ptr<ICanvas> canvas;

public:
    explicit SVGRenderer(std::shared_ptr<ICanvas> c) : canvas(c) {}

    // Original shapes
    void visit(const Circle& circle) override;
    void visit(const Rectangle& rect) override;
    void visit(const Text& text) override;
    void visit(const Image& image) override;

    // New shapes
    void visit(const Line& line) override;
    void visit(const Square& square) override;
    void visit(const Triangle& triangle) override;
    void visit(const Ellipse& ellipse) override;
    void visit(const Polygon& polygon) override;

    std::shared_ptr<ICanvas> getCanvas() const { return canvas; }
};

