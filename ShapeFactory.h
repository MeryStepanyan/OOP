#pragma once
#include "IShape.h"
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

	void registerShape(const std::string& typeName, std::function<std::shared_ptr<IShape>()> creator);

private:
	ShapeFactory();
	std::map<std::string, std::function<std::shared_ptr<IShape>()>> creators;
};