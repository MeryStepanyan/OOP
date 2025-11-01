#pragma once
#include <algorithm>
#include <cmath>

struct Point
{
	float x;
	float y;

	Point() : x(0.0f), y(0.0f) {}
	Point(float x, float y) : x(x), y(y) {}


	float distanceTo(const Point& other) const {
		float dx = x - other.x;
		float dy = y - other.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	Point operator+(const Point& other) const {
		return Point(x + other.x, y + other.y);
	}

	Point operator-(const Point& other) const {
		return Point(x - other.x, y - other.y);
	}
};


	class BoundingBox
	{
	public:
		BoundingBox() : topLeft(0.0f, 0.0f), bottomRight(0.0f, 0.0f) {}

		BoundingBox(Point tl, Point br) : topLeft(tl), bottomRight(br) {
			normalize();
		}

		BoundingBox(float x1, float y1, float x2, float y2)
			: topLeft(x1, y1), bottomRight(x2, y2) {
			normalize();
		}


		Point getTopLeft() const { return topLeft; }
		Point getBottomRight() const { return bottomRight; }
		Point getCenter() const {
			return Point((topLeft.x + bottomRight.x) / 2.0f,
				(topLeft.y + bottomRight.y) / 2.0f);
		}

		float getWidth() const { return bottomRight.x - topLeft.x; }
		float getHeight() const { return topLeft.y - bottomRight.y; }
		float getArea() const { return getWidth() * getHeight(); }


		void setTopLeft(const Point& p) {
			topLeft = p;
			normalize();
		}

		void setBottomRight(const Point& p) {
			bottomRight = p;
			normalize();
		}

		void setXTopLeft(float x) {
			topLeft.x = x;
			normalize();
		}

		void setYTopLeft(float y) {
			topLeft.y = y;
			normalize();
		}

		void setXBottomRight(float x) {
			bottomRight.x = x;
			normalize();
		}

		void setYBottomRight(float y) {
			bottomRight.y = y;
			normalize();
		}


		bool contains(const Point& p) const {
			return p.x >= topLeft.x && p.x <= bottomRight.x &&
				p.y <= topLeft.y && p.y >= bottomRight.y;
		}

		bool intersects(const BoundingBox& other) const {
			return !(bottomRight.x < other.topLeft.x ||
				topLeft.x > other.bottomRight.x ||
				bottomRight.y > other.topLeft.y ||
				topLeft.y < other.bottomRight.y);
		}

		bool isValid() const {
			return topLeft.x <= bottomRight.x && topLeft.y >= bottomRight.y;
		}

		void expand(float amount) {
			topLeft.x -= amount;
			topLeft.y += amount;
			bottomRight.x += amount;
			bottomRight.y -= amount;
		}

	private:
		Point topLeft;     
		Point bottomRight; 

		
		void normalize() {
			if (topLeft.x > bottomRight.x) {
				std::swap(topLeft.x, bottomRight.x);
			}
			if (topLeft.y < bottomRight.y) {
				std::swap(topLeft.y, bottomRight.y);
			}
		}
	};
