#include "Geometry.h"



	Point::Point() : x(0.0f), y(0.0f) {}
	Point::Point(float x, float y) : x(x), y(y) {}


	float Point::distanceTo(const Point& other) const {
		float dx = x - other.x;
		float dy = y - other.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	Point Point::operator+(const Point& other) const {
		return Point(x + other.x, y + other.y);
	}

	Point Point::operator-(const Point& other) const {
		return Point(x - other.x, y - other.y);
	}



	BoundingBox::BoundingBox() : topLeft(0.0f, 0.0f), bottomRight(0.0f, 0.0f) {}

	BoundingBox::BoundingBox(Point tl, Point br) : topLeft(tl), bottomRight(br) {
		normalize();
	}

	BoundingBox::BoundingBox(float x1, float y1, float x2, float y2)
		: topLeft(x1, y1), bottomRight(x2, y2) {
		normalize();
	}


	Point BoundingBox::getTopLeft() const { return topLeft; }
	Point BoundingBox::getBottomRight() const { return bottomRight; }
	Point BoundingBox::getCenter() const {
		return Point((topLeft.x + bottomRight.x) / 2.0f,
			(topLeft.y + bottomRight.y) / 2.0f);
	}

	float BoundingBox::getWidth() const { return bottomRight.x - topLeft.x; }
	float BoundingBox::getHeight() const { return topLeft.y - bottomRight.y; }
	float BoundingBox::getArea() const { return getWidth() * getHeight(); }


	void BoundingBox::setTopLeft(const Point& p) {
		topLeft = p;
		normalize();
	}

	void BoundingBox::setBottomRight(const Point& p) {
		bottomRight = p;
		normalize();
	}

	void BoundingBox::setXTopLeft(float x) {
		topLeft.x = x;
		normalize();
	}

	void BoundingBox::setYTopLeft(float y) {
		topLeft.y = y;
		normalize();
	}

	void BoundingBox::setXBottomRight(float x) {
		bottomRight.x = x;
		normalize();
	}

	void BoundingBox::setYBottomRight(float y) {
		bottomRight.y = y;
		normalize();
	}


	bool BoundingBox::contains(const Point& p) const {
		return p.x >= topLeft.x && p.x <= bottomRight.x &&
			p.y <= topLeft.y && p.y >= bottomRight.y;
	}

	bool BoundingBox::intersects(const BoundingBox& other) const {
		return !(bottomRight.x < other.topLeft.x ||
			topLeft.x > other.bottomRight.x ||
			bottomRight.y > other.topLeft.y ||
			topLeft.y < other.bottomRight.y);
	}

	bool BoundingBox::isValid() const {
		return topLeft.x <= bottomRight.x && topLeft.y >= bottomRight.y;
	}

	void BoundingBox::expand(float amount) {
		topLeft.x -= amount;
		topLeft.y += amount;
		bottomRight.x += amount;
		bottomRight.y -= amount;
	}

	void BoundingBox::normalize() {
		if (topLeft.x > bottomRight.x) {
			std::swap(topLeft.x, bottomRight.x);
		}
		if (topLeft.y < bottomRight.y) {
			std::swap(topLeft.y, bottomRight.y);
		}
	}
