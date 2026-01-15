#pragma once
#include "BaseCommand.h"
#include <vector>
#include <memory>
#include "BaseCommand.h"
#include "ActionFactory.h"
#include "StylePresets.h"
#include "StyledShape.h"
#include "Editor.h"
#include "Shape.h"
#include "Pen.h"
#include "SolidBrush.h"
#include <stdexcept>
#include <iostream>
#include <sstream>

class AddShapeCommand : public BaseCommand {
public:
    AddShapeCommand();
    explicit AddShapeCommand(const std::unordered_map<std::string, std::string>& args);

    std::string getName() const override;
    void execute(Editor& editor) override;

private:
    ShapeStyle createStyleWithColors(const std::string& type);
    ShapeStyle createStyleFromPreset(const std::string& type);
    std::shared_ptr<IShape> createShape(const std::string& type);

    float parseFloat(const std::string& key, float defaultValue = 0.0f) const;
    std::vector<Point> parsePoints(const std::string& key) const;

    // shape creators


    // ========================================================================
    // SHAPE CREATION METHODS
    // ========================================================================

    std::shared_ptr<Circle> createCircle();
    std::shared_ptr<Rectangle> createRectangle();
    std::shared_ptr<Text> createText();
    std::shared_ptr<Image> createImage();
    std::shared_ptr<Line> createLine();
    std::shared_ptr<Square> createSquare();
    std::shared_ptr<Triangle> createTriangle();
  
    std::shared_ptr<Ellipse> createEllipse();
    std::shared_ptr<Polygon> createPolygon();
};