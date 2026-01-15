#include "ShapeFactory.h"

ShapeFactory::ShapeFactory()
{
	registerShape("Circle", []() { return std::make_shared<Circle>(); });
	registerShape("Rectangle", []() { return std::make_shared<Rectangle>(); });
	registerShape("Text", []() { return std::make_shared<Text>(); });
	registerShape("Image", []() { return std::make_shared<Image>(); });
	registerShape("Square", []() { return std::make_shared<Square>(); });
	registerShape("Line", []() { return std::make_shared<Line>(); });
	registerShape("Triangle", []() { return std::make_shared<Triangle>(); });
	registerShape("Ellipse", []() { return std::make_shared<Ellipse>(); });
	registerShape("Polygon", []() { return std::make_shared<Polygon>(std::vector<Point>{{0, 0}, { 0,1 }, { 1,0 }}); }); 

}

void ShapeFactory::registerShape(const std::string& typeName,
	std::function<std::shared_ptr<IShape>()> creator)
{
	creators[typeName] = creator;
}

std::shared_ptr<IShape> ShapeFactory::createShape(IShape::ShapeType type)
{
	switch (type)
	{
	case IShape::ShapeType::Circle:
		return std::make_shared<Circle>();
	case IShape::ShapeType::Rectangle:
		return std::make_shared<Rectangle>();
	case IShape::ShapeType::Text:
		return std::make_shared<Text>();
	case IShape::ShapeType::Image:
		return std::make_shared<Image>();
	case IShape::ShapeType::Triangle:
		return std::make_shared<Triangle>();
	case IShape::ShapeType::Ellipse:
		return std::make_shared<Ellipse>();
	case IShape::ShapeType::Line:
		return std::make_shared<Line>();
	case IShape::ShapeType::Polygon:
		return std::make_shared<Polygon>();
	case IShape::ShapeType::Square:
		return std::make_shared<Square>();
	default:
		throw std::invalid_argument("Unknown shape type");
	}
}

std::shared_ptr<IShape> ShapeFactory::createShape(const std::string& typeName)
{
	auto& creators = instance().creators;
	auto it = creators.find(typeName);
	if (it != creators.end())
	{
		return it->second();
	}
	throw std::invalid_argument("Unknown shape type: " + typeName);
}

std::shared_ptr<IShape> ShapeFactory::createCircle(float radius, float x, float y)
{
	return std::make_shared<Circle>(radius, x, y);
}

std::shared_ptr<IShape> ShapeFactory::createRectangle(float width, float height, float x, float y)
{
	return std::make_shared<Rectangle>(width, height, x, y);
}

std::shared_ptr<IShape> ShapeFactory::createText(const std::string& content, float fontSize, float x, float y)
{
	return std::make_shared<Text>(content, fontSize, x, y);
}

std::shared_ptr<IShape> ShapeFactory::createImage(const std::string& filePath, float width, float height, float x, float y)
{
	return std::make_shared<Image>(filePath, width, height, x, y);
}

std::shared_ptr<IShape>ShapeFactory::createSquare(float size, float x , float y ) {
	return std::make_shared<Square>(size, x, y);
}

std::shared_ptr<IShape>ShapeFactory::createLine(float x1 , float y1, float x2, float y2 ) {
	return std::make_shared<Line>(x1, y1, x2, y2);
}

std::shared_ptr<IShape>ShapeFactory::createTriangle(float x1 , float y1 ,float x2 , float y2 ,float x3 , float y3) {
	return std::make_shared<Triangle>(x1, y1, x2, y2, x3, y3);
}

std::shared_ptr<IShape>ShapeFactory::createEllipse(float rx, float ry , float x , float y) {
	return std::make_shared<Ellipse>(rx, ry, x, y);
}

std::shared_ptr<IShape>ShapeFactory::createPolygon(const std::vector<Point>& points) {
	return std::make_shared<Polygon>(points);
}