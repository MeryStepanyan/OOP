#include "IShape.h"


void Circle::draw() const
{
	std::cout << "Drawing Circle at (" << center.x << ", " << center.y
		<< ") with radius " << radius << std::endl;
}

void Circle::setPosition(float x, float y)
{
	validateFinite(x, "X coordinate");
	validateFinite(y, "Y coordinate");
	center.x = x;
	center.y = y;
	updateBoundingBox();
}

void Circle::setRadius(float r)
{
	validatePositive(r, "Radius");
	radius = r;
	updateBoundingBox();
}

void Circle::updateBoundingBox()
{
	box = BoundingBox(
		center.x - radius, center.y + radius,
		center.x + radius, center.y - radius
	);
}

void Rectangle::draw() const
{
	Point topLeft = box.getTopLeft();
	std::cout << "Drawing Rectangle at (" << topLeft.x << ", " << topLeft.y
		<< ") with width " << width << " and height " << height << std::endl;
}

void Rectangle::setPosition(float x, float y)
{
	validateFinite(x, "X coordinate");
	validateFinite(y, "Y coordinate");
	box.setXTopLeft(x);
	box.setYTopLeft(y);
	updateBoundingBox();
}

void Rectangle::setWidth(float w)
{
	validatePositive(w, "Width");
	width = w;
	updateBoundingBox();
}

void Rectangle::setHeight(float h)
{
	validatePositive(h, "Height");
	height = h;
	updateBoundingBox();
}

void Rectangle::updateBoundingBox()
{
	Point topLeft = box.getTopLeft();
	box.setXBottomRight(topLeft.x + width);
	box.setYBottomRight(topLeft.y - height);
}
void Text::draw() const
{
	Point topLeft = box.getTopLeft();
	std::cout << "Drawing Text \"" << content << "\" at ("
		<< topLeft.x << ", " << topLeft.y
		<< ") with font size " << fontSize << std::endl;
}

void Text::setPosition(float x, float y)
{
	validateFinite(x, "X coordinate");
	validateFinite(y, "Y coordinate");
	box.setXTopLeft(x);
	box.setYTopLeft(y);
	updateBoundingBox();
}

void Text::setContent(const std::string& text)
{
	content = text;
	updateBoundingBox();
}

void Text::setFontSize(float size)
{
	validatePositive(size, "Font size");
	fontSize = size;
	updateBoundingBox();
}

void Text::updateBoundingBox()
{
	Point topLeft = box.getTopLeft();
	float boxWidth = content.length() * fontSize * 0.6f;
	float boxHeight = fontSize * 1.2f;

	box.setXBottomRight(topLeft.x + boxWidth);
	box.setYBottomRight(topLeft.y - boxHeight);
}