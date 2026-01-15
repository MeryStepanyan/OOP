#pragma once
#include "IShape.h"

// ============================================================================
// EXISTING SHAPES (Circle, Rectangle, Text, Image)
// ============================================================================

class Circle : public IShape {
private:
    Point center;
    float radius;
    void updateBoundingBox();


public:
    Circle(float r = 10.0f, float x = 0.0f, float y = 0.0f);

    void draw() const override;
    void setPosition(float x, float y) override;
    void setRadius(float r);

    ShapeType getType() const override { return ShapeType::Circle; }
    void accept(IShapeVisitor& visitor) const override;
    BoundingBox getBoundingBox() const override { return box; }
    Point getPosition() const override { return center; }

    float getRadius() const { return radius; }
};

class Rectangle : public IShape {
private:
    float width;
    float height;
    void updateBoundingBox();

public:
    Rectangle(float w = 10.0f, float h = 10.0f, float x = 0.0f, float y = 0.0f);

    void draw() const override;
    void setPosition(float x, float y) override;
    void setWidth(float w);
    void setHeight(float h);

    ShapeType getType() const override { return ShapeType::Rectangle; }
    void accept(IShapeVisitor& visitor) const override;
    BoundingBox getBoundingBox() const override { return box; }
    Point getPosition() const override { return box.getTopLeft(); }

    float getWidth() const { return width; }
    float getHeight() const { return height; }
};

class Text : public IShape {
private:
    std::string content;
    float fontSize;
    void updateBoundingBox();

public:
    Text(const std::string& text = "", float size = 12.0f, float x = 0.0f, float y = 0.0f);

    void draw() const override;
    void setPosition(float x, float y) override;
    void setContent(const std::string& text);
    void setFontSize(float size);

    ShapeType getType() const override { return ShapeType::Text; }
    void accept(IShapeVisitor& visitor) const override;
    BoundingBox getBoundingBox() const override { return box; }
    Point getPosition() const override { return box.getTopLeft(); }

    std::string getContent() const { return content; }
    float getFontSize() const { return fontSize; }
};

class Image : public IShape {
private:
    std::string filePath;
    Point position;
    float width;
    float height;
    void updateBoundingBox();

public:
    Image(const std::string& path = "", float w = 100.0f, float h = 100.0f,
        float x = 0.0f, float y = 0.0f);

    void draw() const override;
    void setPosition(float x, float y) override;
    void setFilePath(const std::string& path);
    void setWidth(float w);
    void setHeight(float h);

    ShapeType getType() const override { return ShapeType::Image; }
    void accept(IShapeVisitor& visitor) const override;
    BoundingBox getBoundingBox() const override { return box; }
    Point getPosition() const override { return position; }

    std::string getFilePath() const { return filePath; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
};

// ============================================================================
// NEW SHAPES
// ============================================================================

// Line class
class Line : public IShape {
private:
    Point start;
    Point end;
    void updateBoundingBox();

public:
    Line(float x1 = 0.0f, float y1 = 0.0f, float x2 = 100.0f, float y2 = 100.0f);

    void draw() const override;
    void setPosition(float x, float y) override;
    void setStart(float x, float y);
    void setEnd(float x, float y);

    ShapeType getType() const override { return ShapeType::Line; }
    void accept(IShapeVisitor& visitor) const override;
    BoundingBox getBoundingBox() const override { return box; }
    Point getPosition() const override { return start; }

    Point getStart() const { return start; }
    Point getEnd() const { return end; }
};

// Square class (special case of Rectangle)
class Square : public IShape {
private:
    float size;
    void updateBoundingBox();

public:
    Square(float s = 10.0f, float x = 0.0f, float y = 0.0f);

    void draw() const override;
    void setPosition(float x, float y) override;
    void setSize(float s);

    ShapeType getType() const override { return ShapeType::Square; }
    void accept(IShapeVisitor& visitor) const override;
    BoundingBox getBoundingBox() const override { return box; }
    Point getPosition() const override { return box.getTopLeft(); }

    float getSize() const { return size; }
};

// Triangle class
class Triangle : public IShape {
private:
    Point vertex1, vertex2, vertex3;
    void updateBoundingBox();

public:
    Triangle(float x1 = 0.0f, float y1 = 0.0f,
        float x2 = 50.0f, float y2 = 100.0f,
        float x3 = 100.0f, float y3 = 0.0f);

    void draw() const override;
    void setPosition(float x, float y) override;
    void setVertices(float x1, float y1, float x2, float y2, float x3, float y3);

    ShapeType getType() const override { return ShapeType::Triangle; }
    void accept(IShapeVisitor& visitor) const override;
    BoundingBox getBoundingBox() const override { return box; }
    Point getPosition() const override { return vertex1; }

    Point getVertex1() const { return vertex1; }
    Point getVertex2() const { return vertex2; }
    Point getVertex3() const { return vertex3; }
};

// Ellipse class
class Ellipse : public IShape {
private:
    Point center;
    float radiusX;
    float radiusY;
    void updateBoundingBox();

public:
    Ellipse(float rx = 20.0f, float ry = 10.0f, float x = 0.0f, float y = 0.0f);

    void draw() const override;
    void setPosition(float x, float y) override;
    void setRadii(float rx, float ry);

    ShapeType getType() const override { return ShapeType::Ellipse; }
    void accept(IShapeVisitor& visitor) const override;
    BoundingBox getBoundingBox() const override { return box; }
    Point getPosition() const override { return center; }

    float getRadiusX() const { return radiusX; }
    float getRadiusY() const { return radiusY; }
};

// Polygon class - for custom shapes with any number of vertices
class Polygon : public IShape {
private:
    std::vector<Point> vertices;
    void updateBoundingBox();

public:
    Polygon() = default;

    explicit Polygon(const std::vector<Point>& points);

    void draw() const override;
    void setPosition(float x, float y) override;
    void setVertices(const std::vector<Point>& points);
    void addVertex(const Point& p);

    ShapeType getType() const override { return ShapeType::Polygon; }
    void accept(IShapeVisitor& visitor) const override;
    BoundingBox getBoundingBox() const override { return box; }
    Point getPosition() const override {
        return vertices.empty() ? Point() : vertices[0];
    }

    const std::vector<Point>& getVertices() const { return vertices; }
    size_t getVertexCount() const { return vertices.size(); }
};