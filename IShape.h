#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <limits>

#include "BoundingBox.h"

inline bool isValidFloat(float value) {
	return !std::isnan(value) && !std::isinf(value);
}

inline bool isPositive(float value) {
	return isValidFloat(value) && value > 0.0f;
}

inline void validatePositive(float value, const char* name) {
	if (!isPositive(value)) {
		throw std::invalid_argument(
			std::string(name) + " must be a positive finite number");
	}
}

inline void validateFinite(float value, const char* name) {
	if (!isValidFloat(value)) {
		throw std::invalid_argument(
			std::string(name) + " must be a finite number");
	}
}

class IShape
{
public:
	enum class ShapeType
	{
		Circle,
		Rectangle,
		Text,
		Image
	};

	virtual ~IShape() = default;

	virtual ShapeType getType() const = 0;
	virtual const BoundingBox& getBoundingBox() const = 0;
	virtual const Point& getPosition() const = 0;

	virtual void draw() const = 0;
	virtual void setPosition(float x, float y) = 0;

	virtual void move(float dx, float dy) = 0;
	virtual bool contains(const Point& p) const = 0;
	virtual float getArea() const = 0;

	bool overlaps(const IShape& other) const {
		return getBoundingBox().intersects(other.getBoundingBox());
	}
};

class Circle : public IShape
{
public:
	Circle(float r = 10.0f, float x = 0.0f, float y = 0.0f)
		: radius(r), center(x, y)
	{
		validatePositive(r, "Radius");
		validateFinite(x, "X coordinate");
		validateFinite(y, "Y coordinate");
		updateBoundingBox();
	}

	~Circle() override = default;

	ShapeType getType() const override {
		return ShapeType::Circle;
	}

	const BoundingBox& getBoundingBox() const override {
		return box;
	}

	const Point& getPosition() const override {
		return center;
	}

	void draw() const override;
	void setPosition(float x, float y) override;
	void setRadius(float r);
	float getRadius() const { return radius; }

	void move(float dx, float dy) override {
		validateFinite(dx, "Delta X");
		validateFinite(dy, "Delta Y");
		center.x += dx;
		center.y += dy;
		updateBoundingBox();
	}

	bool contains(const Point& p) const override {
		float dx = p.x - center.x;
		float dy = p.y - center.y;
		return (dx * dx + dy * dy) <= (radius * radius);
	}

	float getArea() const override {
		return 3.14159265359f * radius * radius;
	}

private:
	float radius;
	Point center;
	mutable BoundingBox box;

	void updateBoundingBox();
};

class Rectangle : public IShape
{
public:
	Rectangle(float w = 10.0f, float h = 10.0f, float x = 0.0f, float y = 0.0f)
		: width(w), height(h), box(x, y, x + w, y - h)
	{
		validatePositive(w, "Width");
		validatePositive(h, "Height");
		validateFinite(x, "X coordinate");
		validateFinite(y, "Y coordinate");
	}

	~Rectangle() override = default;

	ShapeType getType() const override {
		return ShapeType::Rectangle;
	}

	const BoundingBox& getBoundingBox() const override {
		return box;
	}

	const Point& getPosition() const override {
		return box.getTopLeft();
	}

	void draw() const override;
	void setPosition(float x, float y) override;
	void setWidth(float w);
	void setHeight(float h);

	float getWidth() const { return width; }
	float getHeight() const { return height; }

	void move(float dx, float dy) override {
		validateFinite(dx, "Delta X");
		validateFinite(dy, "Delta Y");
		Point topLeft = box.getTopLeft();
		setPosition(topLeft.x + dx, topLeft.y + dy);
	}

	bool contains(const Point& p) const override {
		return box.contains(p);
	}

	float getArea() const override {
		return width * height;
	}

private:
	float width;
	float height;
	mutable BoundingBox box;

	void updateBoundingBox();
};

class Text : public IShape
{
public:
	Text(const std::string& content = "", float fontSize = 12.0f,
		float x = 0.0f, float y = 0.0f)
		: content(content), fontSize(fontSize), box(x, y, x, y)
	{
		validatePositive(fontSize, "Font size");
		validateFinite(x, "X coordinate");
		validateFinite(y, "Y coordinate");
		updateBoundingBox();
	}

	~Text() override = default;

	ShapeType getType() const override {
		return ShapeType::Text;
	}

	const BoundingBox& getBoundingBox() const override {
		return box;
	}

	const Point& getPosition() const override {
		return box.getTopLeft();
	}

	void draw() const override;
	void setPosition(float x, float y) override;
	void setContent(const std::string& text);
	void setFontSize(float size);

	const std::string& getContent() const { return content; }
	float getFontSize() const { return fontSize; }

	void move(float dx, float dy) override {
		validateFinite(dx, "Delta X");
		validateFinite(dy, "Delta Y");
		Point topLeft = box.getTopLeft();
		setPosition(topLeft.x + dx, topLeft.y + dy);
	}

	bool contains(const Point& p) const override {
		return box.contains(p);
	}

	float getArea() const override {
		return box.getArea();
	}

private:
	std::string content;
	float fontSize;
	mutable BoundingBox box;

	void updateBoundingBox();
};