#include "Slide.h"
#include <stdexcept>

void Slide::addShape(std::shared_ptr<IShape> shape)
{
	if (!shape) {
		throw std::invalid_argument("Cannot add null shape");
	}
	objects.push_back(shape);
}

void Slide::removeShape(size_t index)
{
	if (index >= objects.size()) {
		throw std::out_of_range("Index out of range");
	}
	objects.erase(objects.begin() + index);
}

IShape* Slide::getShape(size_t index)
{
	if (index >= objects.size()) {
		throw std::out_of_range("Index out of range");
	}
	return objects[index].get();
}

const IShape* Slide::getShape(size_t index) const
{
	if (index >= objects.size()) {
		throw std::out_of_range("Index out of range");
	}
	return objects[index].get();
}

int Slide::getId() const
{
	return m_id;
}

void Slide::setId(int id)
{
	m_id = id;
}

size_t Slide::getShapeCount() const
{
	return objects.size();
}

const std::vector<std::shared_ptr<IShape>>& Slide::getShapes() const
{
	return objects;
}