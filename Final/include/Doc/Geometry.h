#pragma once
#include <algorithm>
#include <cmath>

struct Point
{
	float x;
	float y;

	Point() ;
	Point(float x, float y);


	float distanceTo(const Point& other) const;
	Point operator+(const Point& other) const;
	Point operator-(const Point& other) const;
};


class BoundingBox
{
public:
	BoundingBox();

	BoundingBox(Point tl, Point br);

	BoundingBox(float x1, float y1, float x2, float y2);


	Point getTopLeft() const;
	Point getBottomRight() const;
	Point getCenter() const;

	float getWidth() const;
	float getHeight() const;
	float getArea() const;


	void setTopLeft(const Point& p);
	void setBottomRight(const Point& p);

	void setXTopLeft(float x);

	void setYTopLeft(float y);

	void setXBottomRight(float x);

	void setYBottomRight(float y);


	bool contains(const Point& p) const;
	bool intersects(const BoundingBox& other) const;
	bool isValid() const; 

	void expand(float amount);

private:
	Point topLeft;
	Point bottomRight;


	void normalize();
};