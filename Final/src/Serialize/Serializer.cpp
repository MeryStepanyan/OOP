#include "Serializer.h"
#include "ISerializableRepository.h"
#include "Presentation.h"
#include "Slide.h"
#include "Shape.h"
#include "Geometry.h"
#include "StyledShape.h"
#include "ShapeStyle.h"
#include "Pen.h"
#include "SolidBrush.h"
#include "StylePresets.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <algorithm>
#include <cmath>

// ============================================================================
// STRING UTILITIES
// ============================================================================

std::string Serializer::escapeString(const std::string& str) {
    std::string result;
    result.reserve(str.length() + str.length() / 10);

    for (char c : str) {
        switch (c) {
        case '"': result += "\\\""; break;
        case '\\': result += "\\\\"; break;
        case '\n': result += "\\n"; break;
        case '\r': result += "\\r"; break;
        case '\t': result += "\\t"; break;
        default: result += c; break;
        }
    }
    return result;
}

std::string Serializer::unescapeString(const std::string& str) {
    std::string result;
    result.reserve(str.length());

    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '\\' && i + 1 < str.length()) {
            switch (str[i + 1]) {
            case '"': result += '"'; ++i; break;
            case '\\': result += '\\'; ++i; break;
            case 'n': result += '\n'; ++i; break;
            case 'r': result += '\r'; ++i; break;
            case 't': result += '\t'; ++i; break;
            default: result += str[i]; break;
            }
        }
        else {
            result += str[i];
        }
    }
    return result;
}

std::string Serializer::extractValue(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\"\\s*:\\s*";

    // Try to extract string value
    std::regex stringPattern(searchKey + "\"([^\"]*)\"");
    std::smatch match;
    if (std::regex_search(json, match, stringPattern)) {
        return unescapeString(match[1].str());
    }

    // Try to extract number value
    std::regex numberPattern(searchKey + "([+-]?[0-9]*\\.?[0-9]+)");
    if (std::regex_search(json, match, numberPattern)) {
        return match[1].str();
    }

    // Try to extract boolean
    std::regex boolPattern(searchKey + "(true|false)");
    if (std::regex_search(json, match, boolPattern)) {
        return match[1].str();
    }

    return "";
}

std::string Serializer::extractObject(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\"\\s*:\\s*\\{";
    std::regex pattern(searchKey);
    std::smatch match;

    if (!std::regex_search(json, match, pattern)) {
        return "";
    }

    size_t startPos = match.position() + match.length() - 1;  // Position of opening brace
    size_t endPos = startPos;
    int braceCount = 1;

    // Find matching closing brace
    for (size_t i = startPos + 1; i < json.length() && braceCount > 0; ++i) {
        if (json[i] == '{') braceCount++;
        else if (json[i] == '}') braceCount--;
        endPos = i;
    }

    return json.substr(startPos, endPos - startPos + 1);
}

float Serializer::parseFloat(const std::string& str) {
    try {
        return std::stof(str);
    }
    catch (...) {
        return 0.0f;
    }
}

int Serializer::parseInt(const std::string& str) {
    try {
        return std::stoi(str);
    }
    catch (...) {
        return 0;
    }
}

// ============================================================================
// STYLE SERIALIZATION (NEW)
// ============================================================================

std::string Serializer::serializeStyle(const ShapeStyle& style) {
    std::ostringstream oss;
    oss << "\"style\":{";

    bool hasPen = style.hasPen();
    bool hasBrush = style.hasBrush();

    if (!hasPen && !hasBrush) {
        oss << "}";
        return oss.str();
    }

    // Serialize pen (stroke)
    if (hasPen) {
        auto pen = style.getPen();
        oss << "\"pen\":{";
        oss << "\"color\":\"" << escapeString(pen->getColor()) << "\",";
        oss << "\"width\":" << pen->getWidth() << ",";
        oss << "\"opacity\":" << pen->getOpacity() << ",";
        oss << "\"visible\":" << (pen->isVisible() ? "true" : "false");

        // Line style
        oss << ",\"lineStyle\":\"";
        switch (pen->getStyle()) {
        case IPen::LineStyle::Solid: oss << "solid"; break;
        case IPen::LineStyle::Dashed: oss << "dashed"; break;
        case IPen::LineStyle::Dotted: oss << "dotted"; break;
        case IPen::LineStyle::DashDot: oss << "dashdot"; break;
        }
        oss << "\"";

        // Line cap
        oss << ",\"lineCap\":\"";
        switch (pen->getLineCap()) {
        case IPen::LineCap::Flat: oss << "flat"; break;
        case IPen::LineCap::Round: oss << "round"; break;
        case IPen::LineCap::Square: oss << "square"; break;
        }
        oss << "\"";

        // Line join
        oss << ",\"lineJoin\":\"";
        switch (pen->getLineJoin()) {
        case IPen::LineJoin::Miter: oss << "miter"; break;
        case IPen::LineJoin::Round: oss << "round"; break;
        case IPen::LineJoin::Bevel: oss << "bevel"; break;
        }
        oss << "\"";

        oss << "}";
    }

    if (hasPen && hasBrush) {
        oss << ",";
    }

    // Serialize brush (fill)
    if (hasBrush) {
        auto brush = style.getBrush();
        oss << "\"brush\":{";
        oss << "\"color\":\"" << escapeString(brush->getColor()) << "\",";
        oss << "\"opacity\":" << brush->getOpacity() << ",";
        oss << "\"visible\":" << (brush->isVisible() ? "true" : "false");

        // Brush type
        oss << ",\"type\":\"";
        switch (brush->getType()) {
        case IBrush::BrushType::Solid: oss << "solid"; break;
        case IBrush::BrushType::Gradient: oss << "gradient"; break;
        case IBrush::BrushType::Pattern: oss << "pattern"; break;
        case IBrush::BrushType::None: oss << "none"; break;
        }
        oss << "\"";

        oss << "}";
    }

    oss << "}";
    return oss.str();
}

ShapeStyle Serializer::deserializeStyle(const std::string& json) {
    std::shared_ptr<IPen> pen = nullptr;
    std::shared_ptr<IBrush> brush = nullptr;

    // Extract pen object
    std::string penJson = extractObject(json, "pen");
    if (!penJson.empty()) {
        std::string color = extractValue(penJson, "color");
        float width = parseFloat(extractValue(penJson, "width"));
        float opacity = parseFloat(extractValue(penJson, "opacity"));

        pen = std::make_shared<Pen>(color, width, opacity);

        // Load line style
        std::string lineStyle = extractValue(penJson, "lineStyle");
        if (lineStyle == "dashed") pen->setStyle(IPen::LineStyle::Dashed);
        else if (lineStyle == "dotted") pen->setStyle(IPen::LineStyle::Dotted);
        else if (lineStyle == "dashdot") pen->setStyle(IPen::LineStyle::DashDot);

        // Load line cap
        std::string lineCap = extractValue(penJson, "lineCap");
        if (lineCap == "round") pen->setLineCap(IPen::LineCap::Round);
        else if (lineCap == "square") pen->setLineCap(IPen::LineCap::Square);

        // Load line join
        std::string lineJoin = extractValue(penJson, "lineJoin");
        if (lineJoin == "round") pen->setLineJoin(IPen::LineJoin::Round);
        else if (lineJoin == "bevel") pen->setLineJoin(IPen::LineJoin::Bevel);
    }

    // Extract brush object
    std::string brushJson = extractObject(json, "brush");
    if (!brushJson.empty()) {
        std::string color = extractValue(brushJson, "color");
        float opacity = parseFloat(extractValue(brushJson, "opacity"));

        brush = std::make_shared<SolidBrush>(color, opacity);

        // Load brush type (for future gradient/pattern support)
        std::string brushType = extractValue(brushJson, "type");
        if (brushType == "none") brush->setType(IBrush::BrushType::None);
        else if (brushType == "gradient") brush->setType(IBrush::BrushType::Gradient);
        else if (brushType == "pattern") brush->setType(IBrush::BrushType::Pattern);
    }

    return ShapeStyle(pen, brush, "loaded");
}

// ============================================================================
// SHAPE GEOMETRY SERIALIZATION
// ============================================================================

std::string Serializer::serializeCircle(const Circle& circle) {
    std::ostringstream oss;
    oss << "\"type\":\"circle\","
        << "\"radius\":" << circle.getRadius() << ","
        << "\"x\":" << circle.getPosition().x << ","
        << "\"y\":" << circle.getPosition().y;
    return oss.str();
}

std::string Serializer::serializeRectangle(const Rectangle& rect) {
    std::ostringstream oss;
    oss << "\"type\":\"rectangle\","
        << "\"width\":" << rect.getWidth() << ","
        << "\"height\":" << rect.getHeight() << ","
        << "\"x\":" << rect.getPosition().x << ","
        << "\"y\":" << rect.getPosition().y;
    return oss.str();
}

std::string Serializer::serializeText(const Text& text) {
    std::ostringstream oss;
    oss << "\"type\":\"text\","
        << "\"content\":\"" << escapeString(text.getContent()) << "\","
        << "\"fontSize\":" << text.getFontSize() << ","
        << "\"x\":" << text.getPosition().x << ","
        << "\"y\":" << text.getPosition().y;
    return oss.str();
}

std::string Serializer::serializeImage(const Image& image) {
    std::ostringstream oss;
    oss << "\"type\":\"image\","
        << "\"filePath\":\"" << escapeString(image.getFilePath()) << "\","
        << "\"width\":" << image.getWidth() << ","
        << "\"height\":" << image.getHeight() << ","
        << "\"x\":" << image.getPosition().x << ","
        << "\"y\":" << image.getPosition().y;
    return oss.str();
}

std::string Serializer::serializeLine(const Line& line) {
    std::ostringstream oss;
    oss << "\"type\":\"line\","
        << "\"x1\":" << line.getStart().x << ","
        << "\"y1\":" << line.getStart().y << ","
        << "\"x2\":" << line.getEnd().x << ","
        << "\"y2\":" << line.getEnd().y;
    return oss.str();
}

std::string Serializer::serializeTriangle(const Triangle& triangle) {
    std::ostringstream oss;
    oss << "\"type\":\"triangle\","
        << "\"x1\":" << triangle.getVertex1().x << ","
        << "\"y1\":" << triangle.getVertex1().y << ","
        << "\"x2\":" << triangle.getVertex2().x << ","
        << "\"y2\":" << triangle.getVertex2().y << ","
        << "\"x3\":" << triangle.getVertex3().x << ","
        << "\"y3\":" << triangle.getVertex3().y;
    return oss.str();
}

std::string Serializer::serializeEllipse(const Ellipse& ellipse) {
    std::ostringstream oss;
    oss << "\"type\":\"ellipse\","
        << "\"radiusx\":" << ellipse.getRadiusX() << ","
        << "\"radiusy\":" << ellipse.getRadiusY() << ","
        << "\"x\":" << ellipse.getPosition().x << ","
        << "\"y\":" << ellipse.getPosition().y;
    return oss.str();
}

std::string Serializer::serializeSquare(const Square& square) {
    std::ostringstream oss;
    oss << "\"type\":\"square\","
        << "\"size\":" << square.getSize() << ","
        << "\"x\":" << square.getPosition().x << ","
        << "\"y\":" << square.getPosition().y;
    return oss.str();
}

std::string Serializer::serializePolygon(const Polygon& polygon) {
    std::ostringstream oss;
    oss << "\"type\":\"polygon\","
        << "\"points\":\"";

    const auto& vertices = polygon.getVertices();
    for (size_t i = 0; i < vertices.size(); ++i) {
        oss << vertices[i].x << "," << vertices[i].y;
        if (i < vertices.size() - 1) oss << ",";
    }
    oss << "\"";

    return oss.str();
}

// ============================================================================
// STYLED SHAPE SERIALIZATION (NEW - Main Method)
// ============================================================================

std::string Serializer::serializeStyledShape(const StyledShape& styledShape) {
    std::ostringstream oss;
    oss << "{";

    auto shape = styledShape.getShape();
    if (!shape) {
        return "{}";
    }

    // Serialize geometry
    switch (shape->getType()) {
    case IShape::ShapeType::Circle:
        oss << serializeCircle(dynamic_cast<const Circle&>(*shape));
        break;
    case IShape::ShapeType::Rectangle:
        oss << serializeRectangle(dynamic_cast<const Rectangle&>(*shape));
        break;
    case IShape::ShapeType::Text:
        oss << serializeText(dynamic_cast<const Text&>(*shape));
        break;
    case IShape::ShapeType::Image:
        oss << serializeImage(dynamic_cast<const Image&>(*shape));
        break;
    case IShape::ShapeType::Line:
        oss << serializeLine(dynamic_cast<const Line&>(*shape));
        break;
    case IShape::ShapeType::Triangle:
        oss << serializeTriangle(dynamic_cast<const Triangle&>(*shape));
        break;
    case IShape::ShapeType::Ellipse:
        oss << serializeEllipse(dynamic_cast<const Ellipse&>(*shape));
        break;
    case IShape::ShapeType::Square:
        oss << serializeSquare(dynamic_cast<const Square&>(*shape));
        break;
    case IShape::ShapeType::Polygon:
        oss << serializePolygon(dynamic_cast<const Polygon&>(*shape));
        break;
    default:
        return "{}";
    }

    // Serialize style
    const auto& style = styledShape.getStyle();
    if (style.hasPen() || style.hasBrush()) {
        oss << "," << serializeStyle(style);
    }

    oss << "}";
    return oss.str();
}

// ============================================================================
// LEGACY SHAPE SERIALIZATION (for compatibility)
// ============================================================================

std::string Serializer::serializeShape(const IShape& shape) {
    std::ostringstream oss;
    oss << "{";

    switch (shape.getType()) {
    case IShape::ShapeType::Circle:
        oss << serializeCircle(dynamic_cast<const Circle&>(shape));
        break;
    case IShape::ShapeType::Rectangle:
        oss << serializeRectangle(dynamic_cast<const Rectangle&>(shape));
        break;
    case IShape::ShapeType::Text:
        oss << serializeText(dynamic_cast<const Text&>(shape));
        break;
    default:
        return "{}";
    }

    oss << "}";
    return oss.str();
}

// ============================================================================
// SLIDE SERIALIZATION (UPDATED)
// ============================================================================

std::string Serializer::serializeSlide(const Slide& slide) {
    std::ostringstream oss;
    oss << "{"
        << "\"id\":" << slide.getId() << ","
        << "\"shapes\":[";

    for (size_t i = 0; i < slide.getShapeCount(); ++i) {
        if (i > 0) oss << ",";

        // ✅ FIX: Get StyledShape and serialize with style
        StyledShape styledShape = slide.getShape(i);
        oss << serializeStyledShape(styledShape);
    }

    oss << "]}";
    return oss.str();
}

// ============================================================================
// PRESENTATION SERIALIZATION (UPDATED)
// ============================================================================

std::string Serializer::serialize(const Presentation& pres) {
    std::ostringstream oss;
    oss << "{\"version\":\"1.0\",\"slides\":[";

    for (size_t i = 0; i < pres.getSlideCount(); ++i) {
        if (i > 0) oss << ",";
        auto slide = pres.getSlide(i);
        oss << serializeSlide(*slide);
    }

    oss << "]}";
    return oss.str();
}

// ============================================================================
// SHAPE DESERIALIZATION (Geometry only)
// ============================================================================

std::shared_ptr<Circle> Serializer::deserializeCircle(const std::string& json) {
    float radius = parseFloat(extractValue(json, "radius"));
    float x = parseFloat(extractValue(json, "x"));
    float y = parseFloat(extractValue(json, "y"));
    return std::make_shared<Circle>(radius, x, y);
}

std::shared_ptr<Rectangle> Serializer::deserializeRectangle(const std::string& json) {
    float width = parseFloat(extractValue(json, "width"));
    float height = parseFloat(extractValue(json, "height"));
    float x = parseFloat(extractValue(json, "x"));
    float y = parseFloat(extractValue(json, "y"));
    return std::make_shared<Rectangle>(width, height, x, y);
}

std::shared_ptr<Text> Serializer::deserializeText(const std::string& json) {
    std::string content = extractValue(json, "content");
    float fontSize = parseFloat(extractValue(json, "fontSize"));
    float x = parseFloat(extractValue(json, "x"));
    float y = parseFloat(extractValue(json, "y"));
    return std::make_shared<Text>(content, fontSize, x, y);
}

std::shared_ptr<Image> Serializer::deserializeImage(const std::string& json) {
    std::string filePath = extractValue(json, "filePath");
    float width = parseFloat(extractValue(json, "width"));
    float height = parseFloat(extractValue(json, "height"));
    float x = parseFloat(extractValue(json, "x"));
    float y = parseFloat(extractValue(json, "y"));
    return std::make_shared<Image>(filePath, width, height, x, y);
}

std::shared_ptr<Line> Serializer::deserializeLine(const std::string& json) {
    float x1 = parseFloat(extractValue(json, "x1"));
    float y1 = parseFloat(extractValue(json, "y1"));
    float x2 = parseFloat(extractValue(json, "x2"));
    float y2 = parseFloat(extractValue(json, "y2"));
    return std::make_shared<Line>(x1, y1, x2, y2);
}

std::shared_ptr<Triangle> Serializer::deserializeTriangle(const std::string& json) {
    float x1 = parseFloat(extractValue(json, "x1"));
    float y1 = parseFloat(extractValue(json, "y1"));
    float x2 = parseFloat(extractValue(json, "x2"));
    float y2 = parseFloat(extractValue(json, "y2"));
    float x3 = parseFloat(extractValue(json, "x3"));
    float y3 = parseFloat(extractValue(json, "y3"));
    return std::make_shared<Triangle>(x1, y1, x2, y2, x3, y3);
}

std::shared_ptr<Ellipse> Serializer::deserializeEllipse(const std::string& json) {
    float rx = parseFloat(extractValue(json, "radiusx"));
    float ry = parseFloat(extractValue(json, "radiusy"));
    float x = parseFloat(extractValue(json, "x"));
    float y = parseFloat(extractValue(json, "y"));
    return std::make_shared<Ellipse>(rx, ry, x, y);
}

std::shared_ptr<Square> Serializer::deserializeSquare(const std::string& json) {
    float size = parseFloat(extractValue(json, "size"));
    float x = parseFloat(extractValue(json, "x"));
    float y = parseFloat(extractValue(json, "y"));
    return std::make_shared<Square>(size, x, y);
}

std::shared_ptr<Polygon> Serializer::deserializePolygon(const std::string& json) {
    std::string pointsStr = extractValue(json, "points");
    std::vector<Point> points;
    std::istringstream iss(pointsStr);
    float x, y;
    char comma;

    while (iss >> x >> comma >> y) {
        points.push_back(Point(x, y));
        if (iss.peek() == ',') iss.ignore();
    }

    return std::make_shared<Polygon>(points);
}

// ============================================================================
// STYLED SHAPE DESERIALIZATION (NEW - Main Method)
// ============================================================================

StyledShape Serializer::deserializeStyledShape(const std::string& json) {
    std::string type = extractValue(json, "type");

    // Deserialize geometry
    std::shared_ptr<IShape> shape;

    if (type == "circle") shape = deserializeCircle(json);
    else if (type == "rectangle") shape = deserializeRectangle(json);
    else if (type == "text") shape = deserializeText(json);
    else if (type == "image") shape = deserializeImage(json);
    else if (type == "line") shape = deserializeLine(json);
    else if (type == "triangle") shape = deserializeTriangle(json);
    else if (type == "ellipse") shape = deserializeEllipse(json);
    else if (type == "square") shape = deserializeSquare(json);
    else if (type == "polygon") shape = deserializePolygon(json);

    if (!shape) {
        throw std::runtime_error("Failed to deserialize shape of type: " + type);
    }

    // Deserialize style
    std::string styleJson = extractObject(json, "style");
    ShapeStyle style;

    if (!styleJson.empty()) {
        style = deserializeStyle(styleJson);
    }
    else {
        // Apply default style based on type
        if (type == "circle") style = StylePresets::createDefaultCircle();
        else if (type == "rectangle") style = StylePresets::createDefaultRectangle();
        else if (type == "text") style = StylePresets::createDefaultText();
        else if (type == "image") style = StylePresets::createDefaultImage();
        else style = StylePresets::createDefault();
    }

    return StyledShape(shape, style);
}

// Legacy shape deserialization
std::shared_ptr<IShape> Serializer::deserializeShape(const std::string& json) {
    std::string type = extractValue(json, "type");

    if (type == "Circle") return deserializeCircle(json);
    else if (type == "Rectangle") return deserializeRectangle(json);
    else if (type == "Text") return deserializeText(json);

    return nullptr;
}

// ============================================================================
// SLIDE DESERIALIZATION (UPDATED)
// ============================================================================

std::shared_ptr<Slide> Serializer::deserializeSlide(const std::string& json) {
    int id = parseInt(extractValue(json, "id"));
    auto slide = std::make_shared<Slide>(id);

    // Extract shapes array
    std::regex shapesPattern("\"shapes\"\\s*:\\s*\\[([^\\]]*)\\]");
    std::smatch match;
    if (std::regex_search(json, match, shapesPattern)) {
        std::string shapesJson = match[1].str();

        // Parse shape objects
        std::regex shapePattern("\\{[^\\}]+\\}");
        std::sregex_iterator iter(shapesJson.begin(), shapesJson.end(), shapePattern);
        std::sregex_iterator end;

        for (; iter != end; ++iter) {
            std::string shapeJson = iter->str();

            // ✅ FIX: Deserialize as StyledShape (includes style)
            try {
                StyledShape styledShape = deserializeStyledShape(shapeJson);
                slide->addShape(styledShape);
            }
            catch (const std::exception& e) {
                std::cerr << "Warning: Failed to deserialize shape: " << e.what() << std::endl;
            }
        }
    }

    return slide;
}

// ============================================================================
// PRESENTATION DESERIALIZATION (UPDATED)
// ============================================================================

void Serializer::deserialize(Presentation& pres, const std::string& json) {
    // Clear existing slides
    while (pres.getSlideCount() > 0) {
        pres.removeSlide(0);
    }

    // Extract version (for future compatibility)
    std::string version = extractValue(json, "version");
    if (!version.empty() && version != "1.0") {
        std::cerr << "Warning: Unknown JSON version " << version << std::endl;
    }

    // Extract slides array
    std::regex slidesPattern("\"slides\"\\s*:\\s*\\[([^\\]]*)\\]");
    std::smatch match;
    if (std::regex_search(json, match, slidesPattern)) {
        std::string slidesJson = match[1].str();

        // Parse slide objects
        std::regex slidePattern("\\{[^\\}]+\\}");
        std::sregex_iterator iter(slidesJson.begin(), slidesJson.end(), slidePattern);
        std::sregex_iterator end;

        size_t index = 0;
        for (; iter != end; ++iter) {
            std::string slideJson = iter->str();
            auto slide = deserializeSlide(slideJson);
            if (slide) {
                pres.addSlide(index++, slide);
            }
        }
    }
}

// ============================================================================
// FILE I/O
// ============================================================================

bool Serializer::saveToFile(const ISerializableRepository& repository, const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file for writing: " << filename << std::endl;
            return false;
        }

        std::string json = repository.serialize();
        file << json;
        file.close();

        std::cout << "Presentation saved to: " << filename << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving file: " << e.what() << std::endl;
        return false;
    }
}

bool Serializer::loadFromFile(ISerializableRepository& repository, const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file for reading: " << filename << std::endl;
            return false;
        }

        std::ostringstream oss;
        oss << file.rdbuf();
        std::string json = oss.str();

        repository.deserialize(json);

        std::cout << "Presentation loaded from: " << filename << std::endl;
        std::cout << "Loaded " << repository.getSlideCount() << " slides" << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading file: " << e.what() << std::endl;
        return false;
    }
}

// ============================================================================
// SVG EXPORT (Same as before)
// ============================================================================

static std::string escapeXML(const std::string& str) {
    std::string escaped;
    escaped.reserve(str.length() + str.length() / 10);
    for (char c : str) {
        switch (c) {
        case '<': escaped += "&lt;"; break;
        case '>': escaped += "&gt;"; break;
        case '&': escaped += "&amp;"; break;
        case '"': escaped += "&quot;"; break;
        case '\'': escaped += "&apos;"; break;
        default: escaped += c; break;
        }
    }
    return escaped;
}

std::string Serializer::shapeToSVG(const IShape& shape) {
    std::ostringstream svg;

    switch (shape.getType()) {
    case IShape::ShapeType::Circle: {
        const Circle& circle = dynamic_cast<const Circle&>(shape);
        Point pos = circle.getPosition();
        svg << "<circle cx=\"" << pos.x << "\" cy=\"" << pos.y
            << "\" r=\"" << circle.getRadius()
            << "\" fill=\"blue\" stroke=\"black\" stroke-width=\"1\"/>";
        break;
    }
    case IShape::ShapeType::Rectangle: {
        const Rectangle& rect = dynamic_cast<const Rectangle&>(shape);
        Point pos = rect.getPosition();
        svg << "<rect x=\"" << pos.x << "\" y=\"" << pos.y
            << "\" width=\"" << rect.getWidth() << "\" height=\"" << rect.getHeight()
            << "\" fill=\"red\" stroke=\"black\" stroke-width=\"1\"/>";
        break;
    }
    case IShape::ShapeType::Text: {
        const Text& text = dynamic_cast<const Text&>(shape);
        Point pos = text.getPosition();
        svg << "<text x=\"" << pos.x << "\" y=\"" << pos.y
            << "\" font-size=\"" << text.getFontSize()
            << "\" fill=\"black\" font-family=\"Arial\">"
            << escapeXML(text.getContent()) << "</text>";
        break;
    }
    case IShape::ShapeType::Image: {
        const Image& image = dynamic_cast<const Image&>(shape);
        Point pos = image.getPosition();
        svg << "<image x=\"" << pos.x << "\" y=\"" << pos.y
            << "\" width=\"" << image.getWidth() << "\" height=\"" << image.getHeight()
            << "\" href=\"" << escapeXML(image.getFilePath()) << "\"/>";
        break;
    }
    }

    return svg.str();
}

std::string Serializer::slideToSVG(const Slide& slide, size_t slideIndex) {
    std::ostringstream svg;

    svg << "  <g id=\"slide" << slideIndex << "\">\n";
    svg << "    <rect x=\"0\" y=\"0\" width=\"800\" height=\"600\" "
        << "fill=\"#f0f0f0\" stroke=\"#333\" stroke-width=\"2\"/>\n";

    svg << "    <text x=\"400\" y=\"30\" font-size=\"24\" "
        << "text-anchor=\"middle\" fill=\"#333\" font-family=\"Arial\">"
        << "Slide " << slideIndex << "</text>\n";

    for (size_t i = 0; i < slide.getShapeCount(); ++i) {
        StyledShape styledShape = slide.getShape(i);
        auto shape = styledShape.getShape();
        if (shape) {
            svg << "    " << shapeToSVG(*shape) << "\n";
        }
    }

    svg << "  </g>\n";
    return svg.str();
}

std::string Serializer::presentationToSVG(const Presentation& pres) {
    std::ostringstream svg;

    size_t totalHeight = pres.getSlideCount() * 600;
    if (totalHeight == 0) totalHeight = 600;

    svg << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
        << "width=\"800\" height=\"" << totalHeight
        << "\" viewBox=\"0 0 800 " << totalHeight << "\">\n";

    svg << "  <rect width=\"800\" height=\"" << totalHeight
        << "\" fill=\"white\"/>\n";

    for (size_t i = 0; i < pres.getSlideCount(); ++i) {
        auto slide = pres.getSlide(i);
        if (slide) {
            svg << "  <g transform=\"translate(0, " << (i * 600) << ")\">\n";
            svg << slideToSVG(*slide, i);
            svg << "  </g>\n";
        }
    }

    svg << "</svg>\n";
    return svg.str();
}

bool Serializer::exportToSVG(const ISerializableRepository& repository, const std::string& filename) {
    try {
        const Presentation* pres = dynamic_cast<const Presentation*>(&repository);
        if (!pres) {
            std::cerr << "Error: Cannot export non-Presentation repository to SVG" << std::endl;
            return false;
        }

        std::string svgContent = presentationToSVG(*pres);

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file for writing: " << filename << std::endl;
            return false;
        }

        file << svgContent;
        file.close();

        std::cout << "Presentation exported to SVG: " << filename << std::endl;
        std::cout << "Total slides exported: " << pres->getSlideCount() << std::endl;

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error exporting to SVG: " << e.what() << std::endl;
        return false;
    }
}