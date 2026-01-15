#include "Shape.h"

#include "IShapeVisitor.h"
#include <sstream>
#include <iostream>
#include <algorithm>

// ============================================================================
// EXISTING SHAPES IMPLEMENTATION (Circle, Rectangle, Text, Image)
// ============================================================================

// Circle implementation
Circle::Circle(float r , float x , float y )
    : center(x, y), radius(r) {
    validatePositive(r, "Radius");
    updateBoundingBox();
}

void Circle::draw() const {
    std::cout << "Drawing Circle at (" << center.x << ", " << center.y
        << ") with radius " << radius << std::endl;
}

void Circle::setPosition(float x, float y) {
    validateFinite(x, "X coordinate");
    validateFinite(y, "Y coordinate");
    center.x = x;
    center.y = y;
    updateBoundingBox();
}

void Circle::setRadius(float r) {
    validatePositive(r, "Radius");
    radius = r;
    updateBoundingBox();
}

void Circle::updateBoundingBox() {
    box = BoundingBox(
        center.x - radius, center.y + radius,
        center.x + radius, center.y - radius
    );
}

void Circle::accept(IShapeVisitor& visitor) const {
    visitor.visit(*this);
}

// Rectangle implementation
Rectangle::Rectangle(float w , float h , float x, float y)
    : width(w), height(h) {
    validatePositive(w, "Width");
    validatePositive(h, "Height");
    box = BoundingBox(x, y, x + w, y - h);
}

void Rectangle::draw() const {
    Point topLeft = box.getTopLeft();
    std::cout << "Drawing Rectangle at (" << topLeft.x << ", " << topLeft.y
        << ") with width " << width << " and height " << height << std::endl;
}

void Rectangle::setPosition(float x, float y) {
    validateFinite(x, "X coordinate");
    validateFinite(y, "Y coordinate");
    box = BoundingBox(x, y, x + width, y - height);
}

void Rectangle::setWidth(float w) {
    validatePositive(w, "Width");
    width = w;
    updateBoundingBox();
}

void Rectangle::setHeight(float h) {
    validatePositive(h, "Height");
    height = h;
    updateBoundingBox();
}

void Rectangle::updateBoundingBox() {
    Point topLeft = box.getTopLeft();
    box = BoundingBox(topLeft.x, topLeft.y, topLeft.x + width, topLeft.y - height);
}

void Rectangle::accept(IShapeVisitor& visitor) const {
    visitor.visit(*this);
}

// Text implementation
Text::Text(const std::string& text, float size , float x , float y )
    : content(text), fontSize(size) {
    validatePositive(size, "Font size");
    box = BoundingBox(x, y, x, y);
    updateBoundingBox();
}

void Text::draw() const {
    Point topLeft = box.getTopLeft();
    std::cout << "Drawing Text \"" << content << "\" at ("
        << topLeft.x << ", " << topLeft.y
        << ") with font size " << fontSize << std::endl;
}

void Text::setPosition(float x, float y) {
    validateFinite(x, "X coordinate");
    validateFinite(y, "Y coordinate");
    box.setXTopLeft(x);
    box.setYTopLeft(y);
    updateBoundingBox();
}

void Text::setContent(const std::string& text) {
    content = text;
    updateBoundingBox();
}

void Text::setFontSize(float size) {
    validatePositive(size, "Font size");
    fontSize = size;
    updateBoundingBox();
}

void Text::updateBoundingBox() {
    Point topLeft = box.getTopLeft();
    float boxWidth = content.length() * fontSize * 0.6f;
    float boxHeight = fontSize * 1.2f;

    box.setXBottomRight(topLeft.x + boxWidth);
    box.setYBottomRight(topLeft.y - boxHeight);
}

void Text::accept(IShapeVisitor& visitor) const {
    visitor.visit(*this);
}

// Image implementation
Image::Image(const std::string& path, float w , float h ,
    float x , float y )
    : filePath(path), position(x, y), width(w), height(h) {
    validatePositive(w, "Width");
    validatePositive(h, "Height");
    updateBoundingBox();
}

void Image::draw() const {
    std::cout << "Drawing Image \"" << filePath << "\" at ("
        << position.x << ", " << position.y
        << ") with width " << width << " and height " << height << std::endl;
}

void Image::setPosition(float x, float y) {
    validateFinite(x, "X coordinate");
    validateFinite(y, "Y coordinate");
    position.x = x;
    position.y = y;
    updateBoundingBox();
}

void Image::setFilePath(const std::string& path) {
    filePath = path;
}

void Image::setWidth(float w) {
    validatePositive(w, "Width");
    width = w;
    updateBoundingBox();
}

void Image::setHeight(float h) {
    validatePositive(h, "Height");
    height = h;
    updateBoundingBox();
}

void Image::updateBoundingBox() {
    box = BoundingBox(position.x, position.y, position.x + width, position.y - height);
}

void Image::accept(IShapeVisitor& visitor) const {
    visitor.visit(*this);
}

// ============================================================================
// NEW SHAPES IMPLEMENTATION
// ============================================================================

// Line implementation
Line::Line(float x1 , float y1 , float x2 , float y2)
    : start(x1, y1), end(x2, y2) {
    validateFinite(x1, "X1");
    validateFinite(y1, "Y1");
    validateFinite(x2, "X2");
    validateFinite(y2, "Y2");
    updateBoundingBox();
}

void Line::draw() const {
    std::cout << "Drawing Line from (" << start.x << ", " << start.y
        << ") to (" << end.x << ", " << end.y << ")" << std::endl;
}

void Line::setPosition(float x, float y) {
    float dx = x - start.x;
    float dy = y - start.y;
    start.x = x;
    start.y = y;
    end.x += dx;
    end.y += dy;
    updateBoundingBox();
}

void Line::setStart(float x, float y) {
    validateFinite(x, "X1");
    validateFinite(y, "Y1");
    start.x = x;
    start.y = y;
    updateBoundingBox();
}

void Line::setEnd(float x, float y) {
    validateFinite(x, "X2");
    validateFinite(y, "Y2");
    end.x = x;
    end.y = y;
    updateBoundingBox();
}

void Line::updateBoundingBox() {
    float minX = std::min(start.x, end.x);
    float maxX = std::max(start.x, end.x);
    float minY = std::min(start.y, end.y);
    float maxY = std::max(start.y, end.y);
    box = BoundingBox(minX, maxY, maxX, minY);
}

void Line::accept(IShapeVisitor& visitor) const {
    visitor.visit(*this);
}

// Square implementation
Square::Square(float s , float x , float y )
    : size(s) {
    validatePositive(s, "Size");
    box = BoundingBox(x, y, x + s, y - s);
}

void Square::draw() const {
    Point topLeft = box.getTopLeft();
    std::cout << "Drawing Square at (" << topLeft.x << ", " << topLeft.y
        << ") with size " << size << std::endl;
}

void Square::setPosition(float x, float y) {
    validateFinite(x, "X coordinate");
    validateFinite(y, "Y coordinate");
    box = BoundingBox(x, y, x + size, y - size);
}

void Square::setSize(float s) {
    validatePositive(s, "Size");
    size = s;
    updateBoundingBox();
}

void Square::updateBoundingBox() {
    Point topLeft = box.getTopLeft();
    box = BoundingBox(topLeft.x, topLeft.y, topLeft.x + size, topLeft.y - size);
}

void Square::accept(IShapeVisitor& visitor) const {
    visitor.visit(*this);
}

// Triangle implementation
Triangle::Triangle(float x1 , float y1,
    float x2 , float y2 ,
    float x3 , float y3 )
    : vertex1(x1, y1), vertex2(x2, y2), vertex3(x3, y3) {
    updateBoundingBox();
}

void Triangle::draw() const {
    std::cout << "Drawing Triangle with vertices: ("
        << vertex1.x << ", " << vertex1.y << "), ("
        << vertex2.x << ", " << vertex2.y << "), ("
        << vertex3.x << ", " << vertex3.y << ")" << std::endl;
}

void Triangle::setPosition(float x, float y) {
    float dx = x - vertex1.x;
    float dy = y - vertex1.y;
    vertex1.x = x;
    vertex1.y = y;
    vertex2.x += dx;
    vertex2.y += dy;
    vertex3.x += dx;
    vertex3.y += dy;
    updateBoundingBox();
}

void Triangle::setVertices(float x1, float y1, float x2, float y2, float x3, float y3) {
    vertex1 = Point(x1, y1);
    vertex2 = Point(x2, y2);
    vertex3 = Point(x3, y3);
    updateBoundingBox();
}

void Triangle::updateBoundingBox() {
    float minX = std::min({ vertex1.x, vertex2.x, vertex3.x });
    float maxX = std::max({ vertex1.x, vertex2.x, vertex3.x });
    float minY = std::min({ vertex1.y, vertex2.y, vertex3.y });
    float maxY = std::max({ vertex1.y, vertex2.y, vertex3.y });
    box = BoundingBox(minX, maxY, maxX, minY);
}

void Triangle::accept(IShapeVisitor& visitor) const {
    visitor.visit(*this);
}

// Ellipse implementation
Ellipse::Ellipse(float rx , float ry , float x , float y)
    : center(x, y), radiusX(rx), radiusY(ry) {
    validatePositive(rx, "RadiusX");
    validatePositive(ry, "RadiusY");
    updateBoundingBox();
}


void Ellipse::draw() const {
    std::cout << "Drawing Ellipse at (" << center.x << ", " << center.y
        << ") with radii (" << radiusX << ", " << radiusY << ")" << std::endl;
}

void Ellipse::setPosition(float x, float y) {
    validateFinite(x, "X coordinate");
    validateFinite(y, "Y coordinate");
    center.x = x;
    center.y = y;
    updateBoundingBox();
}

void Ellipse::setRadii(float rx, float ry) {
    validatePositive(rx, "RadiusX");
    validatePositive(ry, "RadiusY");
    radiusX = rx;
    radiusY = ry;
    updateBoundingBox();
}

void Ellipse::updateBoundingBox() {
    box = BoundingBox(
        center.x - radiusX, center.y + radiusY,
        center.x + radiusX, center.y - radiusY
    );
}

void Ellipse::accept(IShapeVisitor& visitor) const {
    visitor.visit(*this);
}

// Polygon implementation

Polygon::Polygon(const std::vector<Point>& points) : vertices(points) {
    if (points.size() < 3) {
        throw std::invalid_argument("Polygon must have at least 3 vertices");
    }
    updateBoundingBox();
}

void Polygon::draw() const {
    std::cout << "Drawing Polygon with " << vertices.size() << " vertices" << std::endl;
}

void Polygon::setPosition(float x, float y) {
    if (vertices.empty()) return;

    float dx = x - vertices[0].x;
    float dy = y - vertices[0].y;

    for (auto& vertex : vertices) {
        vertex.x += dx;
        vertex.y += dy;
    }
    updateBoundingBox();
}

void Polygon::setVertices(const std::vector<Point>& points) {
    if (points.size() < 3) {
        throw std::invalid_argument("Polygon must have at least 3 vertices");
    }
    vertices = points;
    updateBoundingBox();
}

void Polygon::addVertex(const Point& p) {
    vertices.push_back(p);
    updateBoundingBox();
}

void Polygon::updateBoundingBox() {
    if (vertices.empty()) {
        box = BoundingBox();
        return;
    }

    float minX = vertices[0].x;
    float maxX = vertices[0].x;
    float minY = vertices[0].y;
    float maxY = vertices[0].y;

    for (const auto& v : vertices) {
        minX = std::min(minX, v.x);
        maxX = std::max(maxX, v.x);
        minY = std::min(minY, v.y);
        maxY = std::max(maxY, v.y);
    }

    box = BoundingBox(minX, maxY, maxX, minY);
}

void Polygon::accept(IShapeVisitor& visitor) const {
    visitor.visit(*this);
}