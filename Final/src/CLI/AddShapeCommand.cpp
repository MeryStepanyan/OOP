
#include "AddShapeCommand.h"
//#include "BaseCommand.h"
//#include "ActionFactory.h"
//#include "StylePresets.h"
//#include "StyledShape.h"
//#include "Editor.h"
//#include "IShape.h"
//#include "Pen.h"
//#include "SolidBrush.h"
//#include <stdexcept>
//#include <iostream>
//#include <sstream>




AddShapeCommand ::  AddShapeCommand() = default;
AddShapeCommand ::  AddShapeCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

std::string AddShapeCommand ::  getName() const { return "AddShape"; }

    void  AddShapeCommand ::execute(Editor & editor) {
        auto& presentation = editor.getPresentation();

        if (presentation.getSlideCount() == 0) {
            throw std::logic_error("Cannot add shape: No slides in presentation. Add a slide first.");
        }

        size_t slideIndex = parseSizeTArgument("slide", presentation.getSlideCount() - 1);

        if (slideIndex >= presentation.getSlideCount()) {
            throw std::out_of_range(
                "Slide index " + std::to_string(slideIndex) + " is out of range. " +
                "Valid range: 0 to " + std::to_string(presentation.getSlideCount() - 1)
            );
        }

        validateRequiredArgument("type");
        std::string type = getArgumentValue("type");

        // Create shape based on type
        std::shared_ptr<IShape> shape = createShape(type);

        // Create style with custom or default colors
        ShapeStyle style = createStyleWithColors(type);

        StyledShape styledShape(shape, style);

        auto action = ActionFactory::createAddShapeAction(slideIndex, styledShape);
        editor.doAction(std::move(action));

        auto slide = presentation.getSlide(slideIndex);
        std::cout << type << " added to slide " << slideIndex
            << " (total shapes: " << slide->getShapeCount() << ")" << std::endl;
    }


    // ========================================================================
    // CUSTOM COLOR SUPPORT
    // ========================================================================

    /**
     * Create style with custom colors or fall back to defaults
     */
    ShapeStyle  AddShapeCommand::createStyleWithColors(const std::string& type) {
        bool hasCustomColors = false;
        std::shared_ptr<IPen> pen;
        std::shared_ptr<IBrush> brush;

        // Check for custom stroke/outline color
        try {
            std::string strokeColor = getArgumentValue("strokecolor");
            float strokeWidth = parseFloat("strokewidth", 2.0f);
            pen = std::make_shared<Pen>(strokeColor, strokeWidth);
            hasCustomColors = true;
            std::cout << "DEBUG: Using custom stroke color: " << strokeColor << std::endl;
        }
        catch (...) {
            // No custom stroke color
        }

        // Check for custom fill color
        try {
            std::string fillColor = getArgumentValue("fillcolor");

            if (fillColor == "none" || fillColor == "transparent") {
                // No fill
                brush = nullptr;
                hasCustomColors = true;
                std::cout << "DEBUG: Using no fill" << std::endl;
            }
            else {
                float opacity = parseFloat("fillopacity", 0.8f);
                brush = std::make_shared<SolidBrush>(fillColor, opacity);
                hasCustomColors = true;
                std::cout << "DEBUG: Using custom fill color: " << fillColor << std::endl;
            }
        }
        catch (...) {
            // No custom fill color
        }

        // Check for text color (for text shapes)
        if (type == "text") {
            try {
                std::string textColor = getArgumentValue("textcolor");
                pen = std::make_shared<Pen>(textColor, 1.0f);
                hasCustomColors = true;
                std::cout << "DEBUG: Using custom text color: " << textColor << std::endl;
            }
            catch (...) {}
        }

        // If custom colors provided, use them
        if (hasCustomColors) {
            // Fill in missing colors with defaults
            if (!pen) {
                pen = std::make_shared<Pen>("#000000", 2.0f);
            }
            if (!brush && type != "text" && type != "line") {
                brush = std::make_shared<SolidBrush>("#CCCCCC", 0.8f);
            }

            return ShapeStyle(pen, brush, "custom");
        }

        // Otherwise, use preset styles
        return createStyleFromPreset(type);
    }

    /**
     * Create style from preset (original behavior)
     */
    ShapeStyle  AddShapeCommand :: createStyleFromPreset(const std::string& type) {
        try {
            std::string styleName = getArgumentValue("style");
            if (styleName == "highlight") {
                if (type == "circle") return StylePresets::createHighlightCircle();
                if (type == "rectangle") return StylePresets::createHighlightRectangle();
            }
            else if (styleName == "success") {
                if (type == "circle") return StylePresets::createSuccessCircle();
            }
            else if (styleName == "warning") {
                if (type == "rectangle") return StylePresets::createWarningRectangle();
            }
            else if (styleName == "professional") {
                return StylePresets::createProfessionalShape();
            }
            else if (styleName == "modern") {
                return StylePresets::createModernShape();
            }
            else if (styleName == "minimal") {
                return StylePresets::createMinimalShape();
            }
        }
        catch (...) {}

        // Default styles
        if (type == "circle") return StylePresets::createDefaultCircle();
        if (type == "rectangle") return StylePresets::createDefaultRectangle();
        if (type == "text") return StylePresets::createDefaultText();
        if (type == "image") return StylePresets::createDefaultImage();
        return StylePresets::createDefault();
    }

    // ========================================================================
    // SHAPE CREATION (Same as before)
    // ========================================================================

    std::shared_ptr<IShape>  AddShapeCommand :: createShape(const std::string& type) {
        if (type == "circle") return createCircle();
        else if (type == "rectangle") return createRectangle();
        else if (type == "text") return createText();
        else if (type == "image") return createImage();
        else if (type == "line") return createLine();
        else if (type == "square") return createSquare();
        else if (type == "triangle") return createTriangle();
        else if (type == "ellipse") return createEllipse();
        else if (type == "polygon") return createPolygon();
        else {
            throw std::invalid_argument(
                "Unknown shape type: " + type +
                ". Valid types: circle, rectangle, text, image, line, square, triangle, ellipse, polygon"
            );
        }
    }

    // ========================================================================
    // HELPER METHODS
    // ========================================================================

    float  AddShapeCommand :: parseFloat(const std::string& key, float defaultValue ) const {
        auto it = arguments.find(key);
        if (it == arguments.end()) return defaultValue;
        try {
            return std::stof(it->second);
        }
        catch (const std::exception&) {
            throw std::invalid_argument("Argument '" + key + "' must be a valid number: " + it->second);
        }
    }

    std::vector<Point>  AddShapeCommand :: parsePoints(const std::string& key) const {
        auto it = arguments.find(key);
        if (it == arguments.end()) {
            throw std::invalid_argument("Missing required argument: " + key);
        }

        std::vector<Point> points;
        std::string coordStr = it->second;
        std::istringstream iss(coordStr);
        float x, y;
        char comma;

        while (iss >> x >> comma >> y) {
            points.push_back(Point(x, y));
            if (iss.peek() == ',') {
                iss.ignore();
            }
        }

        return points;
    }

    // ========================================================================
    // SHAPE CREATION METHODS
    // ========================================================================

    std::shared_ptr<Circle> AddShapeCommand :: createCircle() {
        validateRequiredArgument("radius");
        validateRequiredArgument("x");
        validateRequiredArgument("y");

        float radius = parseFloat("radius");
        float x = parseFloat("x");
        float y = parseFloat("y");

        return std::make_shared<Circle>(radius, x, y);
    }

    std::shared_ptr<Rectangle> AddShapeCommand::createRectangle() {
        validateRequiredArgument("width");
        validateRequiredArgument("height");
        validateRequiredArgument("x");
        validateRequiredArgument("y");

        float width = parseFloat("width");
        float height = parseFloat("height");
        float x = parseFloat("x");
        float y = parseFloat("y");

        return std::make_shared<Rectangle>(width, height, x, y);
    }

    std::shared_ptr<Text> AddShapeCommand::createText() {
        validateRequiredArgument("content");
        validateRequiredArgument("x");
        validateRequiredArgument("y");

        std::string content = getArgumentValue("content");
        float x = parseFloat("x");
        float y = parseFloat("y");
        float fontSize = parseFloat("fontsize", 12.0f);

        return std::make_shared<Text>(content, fontSize, x, y);
    }

    std::shared_ptr<Image> AddShapeCommand::createImage() {
        validateRequiredArgument("path");
        validateRequiredArgument("width");
        validateRequiredArgument("height");
        validateRequiredArgument("x");
        validateRequiredArgument("y");

        std::string path = getArgumentValue("path");
        float width = parseFloat("width");
        float height = parseFloat("height");
        float x = parseFloat("x");
        float y = parseFloat("y");

        return std::make_shared<Image>(path, width, height, x, y);
    }

    std::shared_ptr<Line> AddShapeCommand::createLine() {
        validateRequiredArgument("x1");
        validateRequiredArgument("y1");
        validateRequiredArgument("x2");
        validateRequiredArgument("y2");

        float x1 = parseFloat("x1");
        float y1 = parseFloat("y1");
        float x2 = parseFloat("x2");
        float y2 = parseFloat("y2");

        return std::make_shared<Line>(x1, y1, x2, y2);
    }

    std::shared_ptr<Square> AddShapeCommand::createSquare() {
        validateRequiredArgument("size");
        validateRequiredArgument("x");
        validateRequiredArgument("y");

        float size = parseFloat("size");
        float x = parseFloat("x");
        float y = parseFloat("y");

        return std::make_shared<Square>(size, x, y);
    }

    std::shared_ptr<Triangle> AddShapeCommand::createTriangle() {
        validateRequiredArgument("x1");
        validateRequiredArgument("y1");
        validateRequiredArgument("x2");
        validateRequiredArgument("y2");
        validateRequiredArgument("x3");
        validateRequiredArgument("y3");

        float x1 = parseFloat("x1");
        float y1 = parseFloat("y1");
        float x2 = parseFloat("x2");
        float y2 = parseFloat("y2");
        float x3 = parseFloat("x3");
        float y3 = parseFloat("y3");

        return std::make_shared<Triangle>(x1, y1, x2, y2, x3, y3);
    }

    std::shared_ptr<Ellipse> AddShapeCommand::createEllipse() {
        validateRequiredArgument("radiusx");
        validateRequiredArgument("radiusy");
        validateRequiredArgument("x");
        validateRequiredArgument("y");

        float rx = parseFloat("radiusx");
        float ry = parseFloat("radiusy");
        float x = parseFloat("x");
        float y = parseFloat("y");

        return std::make_shared<Ellipse>(rx, ry, x, y);
    }

    std::shared_ptr<Polygon> AddShapeCommand::createPolygon() {
        validateRequiredArgument("points");

        std::vector<Point> points = parsePoints("points");

        if (points.size() < 3) {
            throw std::invalid_argument("Polygon must have at least 3 points");
        }

        return std::make_shared<Polygon>(points);
    }
