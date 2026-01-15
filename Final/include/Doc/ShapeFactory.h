#pragma once
#include "Shape.h"
#include <memory>
#include <map>
#include <functional>
#include <string>

class ShapeFactory
{
public:
	// Singleton instance
	static ShapeFactory& instance()
	{
		static ShapeFactory factory;
		return factory;
	}

	ShapeFactory(const ShapeFactory&) = delete;
	ShapeFactory& operator=(const ShapeFactory&) = delete;

	static std::shared_ptr<IShape> createShape(IShape::ShapeType type);
	static std::shared_ptr<IShape> createShape(const std::string& typeName);

	static std::shared_ptr<IShape> createCircle(float radius = 10.0f, float x = 0.0f, float y = 0.0f);
	static std::shared_ptr<IShape> createRectangle(float width = 10.0f, float height = 10.0f, float x = 0.0f, float y = 0.0f);
	static std::shared_ptr<IShape> createText(const std::string& content = "", float fontSize = 12.0f, float x = 0.0f, float y = 0.0f);
	static std::shared_ptr<IShape> createImage(const std::string& filePath = "", float width = 100.0f, float height = 100.0f, float x = 0.0f, float y = 0.0f);

	static std::shared_ptr<IShape> createSquare(float size = 10.0f, float x = 0.0f, float y = 0.0f);


	static std::shared_ptr<IShape> createLine(float x1 = 0.0f, float y1 = 0.0f, float x2 = 100.0f, float y2 = 100.0f);
	static std::shared_ptr<IShape> createTriangle(float x1 = 0.0f, float y1 = 0.0f,
		float x2 = 50.0f, float y2 = 100.0f,
		float x3 = 100.0f, float y3 = 0.0f);
	

	static std::shared_ptr<IShape> createEllipse(float rx = 20.0f, float ry = 10.0f, float x = 0.0f, float y = 0.0f);

	static std::shared_ptr<IShape> createPolygon(const std::vector<Point>& points);
	void registerShape(const std::string& typeName, std::function<std::shared_ptr<IShape>()> creator);

private:
	ShapeFactory();
	std::map<std::string, std::function<std::shared_ptr<IShape>()>> creators;
};