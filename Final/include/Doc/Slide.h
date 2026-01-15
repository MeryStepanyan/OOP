#pragma once
#include "StyledShape.h" 
#include <vector>
#include <memory>
#include <stdexcept>

class Slide
{
public:
    Slide() : m_id(0) {}
    explicit Slide(int id) : m_id(id) {}
    ~Slide() = default;

    
    void addShape(const StyledShape& shape);
    void removeShape(size_t index);

 
    StyledShape getShape(size_t index) const;

    int getId() const;
    void setId(int id);
    size_t getShapeCount() const;

    const std::vector<StyledShape>& getShapes() const;

private:
    int m_id;
    std::vector<StyledShape> objects;
};