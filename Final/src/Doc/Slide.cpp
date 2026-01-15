#include "Slide.h"

void Slide::addShape(const StyledShape& shape) {
    if (!shape.isValid()) {
        throw std::invalid_argument("Cannot add invalid styled shape to slide");
    }
    objects.push_back(shape);
}

void Slide::removeShape(size_t index) {
    if (index >= objects.size()) {
        throw std::out_of_range("Shape index out of range");
    }
    objects.erase(objects.begin() + index);
}

StyledShape Slide::getShape(size_t index) const {
    if (index >= objects.size()) {
        throw std::out_of_range("Shape index out of range");
    }
    return objects[index];
}

int Slide::getId() const {
    return m_id;
}

void Slide::setId(int id) {
    m_id = id;
}

size_t Slide::getShapeCount() const {
    return objects.size();
}

const std::vector<StyledShape>& Slide::getShapes() const {
    return objects;
}