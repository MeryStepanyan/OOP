#include "ShapeFactory.h"

ShapeFactory::ShapeFactory()
{
	registerShape("Circle", []() { return std::make_shared<Circle>(); });
	registerShape("Rectangle", []() { return std::make_shared<Rectangle>(); });
	registerShape("Text", []() { return std::make_shared<Text>(); });
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