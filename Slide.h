#pragma once
#include "IShape.h"
#include <vector>
#include <memory>

class Slide
{
public:
	Slide() : m_id(0) {}
	explicit Slide(int id) : m_id(id) {}
	~Slide() = default;

	void addShape(std::shared_ptr<IShape> shape);
	void removeShape(size_t index);
	
	IShape* getShape(size_t index);
	const IShape* getShape(size_t index) const;
	
	int getId() const;
	void setId(int id);
	size_t getShapeCount() const;
	const std::vector<std::shared_ptr<IShape>>& getShapes() const;

private:
	int m_id;
	std::vector<std::shared_ptr<IShape>> objects;
};