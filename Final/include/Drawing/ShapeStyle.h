#pragma once
#include "IPen.h"
#include "IBrush.h"
#include <memory>
#include <string>

/**
 * ShapeStyle - Encapsulates visual style properties
 *

 * This class separates appearance from geometry.
 * A shape is pure data (geometry), while style is pure appearance.
 *
 * Usage:
 *   auto pen = std::make_shared<Pen>("#0000FF", 2.0f);
 *   auto brush = std::make_shared<SolidBrush>("#FF0000", 0.8f);
 *   ShapeStyle style(pen, brush, "my-style");
 */
class ShapeStyle {
private:
    std::shared_ptr<IPen> pen;      // Outline/stroke properties
    std::shared_ptr<IBrush> brush;  // Fill properties
    std::string name;               // Style identifier

public:

    ShapeStyle();

   
    ShapeStyle(std::shared_ptr<IPen> p,
        std::shared_ptr<IBrush> b,
        const std::string& n = "custom");

    // Getters
    std::shared_ptr<IPen> getPen() const;
    std::shared_ptr<IBrush> getBrush() const;
    std::string getName() const;

    // Setters
    void setPen(std::shared_ptr<IPen> p); 
    void setBrush(std::shared_ptr<IBrush> b);
    void setName(const std::string& n);


    ShapeStyle clone() const;

    
    bool hasPen() const;

  
    bool hasBrush() const;

    bool isValid() const;
};