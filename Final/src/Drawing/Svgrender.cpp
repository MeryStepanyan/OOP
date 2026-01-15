// SVGRenderer.cpp implementation
#include "SVGRenderer.h"
#include "Shape.h"
#include "Geometry.h"

// Original shapes
void SVGRenderer::visit(const Circle& circle) {
    Point pos = circle.getPosition();
    canvas->drawCircle(pos.x, pos.y, circle.getRadius());
}

void SVGRenderer::visit(const Rectangle& rect) {
    Point pos = rect.getPosition();
    canvas->drawRectangle(pos.x, pos.y, rect.getWidth(), rect.getHeight());
}

void SVGRenderer::visit(const Text& text) {
    Point pos = text.getPosition();
    canvas->drawText(pos.x, pos.y, text.getContent(), text.getFontSize());
}

void SVGRenderer::visit(const Image& image) {
    Point pos = image.getPosition();
    canvas->drawRectangle(pos.x, pos.y, image.getWidth(), image.getHeight());
    canvas->drawText(pos.x + 5, pos.y + 15,
        "[Image: " + image.getFilePath() + "]", 10.0f);
}

// New shapes
void SVGRenderer::visit(const Line& line) {
    Point start = line.getStart();
    Point end = line.getEnd();
    canvas->drawLine(start.x, start.y, end.x, end.y);
}

void SVGRenderer::visit(const Square& square) {
    Point pos = square.getPosition();
    float size = square.getSize();
    canvas->drawRectangle(pos.x, pos.y, size, size);
}

void SVGRenderer::visit(const Triangle& triangle) {
    std::vector<Point> points = {
        triangle.getVertex1(),
        triangle.getVertex2(),
        triangle.getVertex3()
    };
    canvas->drawPolygon(points);
}

void SVGRenderer::visit(const Ellipse& ellipse) {
    Point pos = ellipse.getPosition();
    canvas->drawEllipse(pos.x, pos.y, ellipse.getRadiusX(), ellipse.getRadiusY());
}

void SVGRenderer::visit(const Polygon& polygon) {
    canvas->drawPolygon(polygon.getVertices());
}