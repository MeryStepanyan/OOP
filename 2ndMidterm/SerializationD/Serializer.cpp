//Serializer.cpp
#include "Serializer.h"
#include "ISerializableRepository.h"
#include "Presentation.h"
#include "Slide.h"
#include "IShape.h"
#include "Geometry.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <algorithm>
#include <cmath>

// Escape special characters in JSON strings
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

    return "";
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

// Serialize Circle
std::string Serializer::serializeCircle(const Circle& circle) {
    std::ostringstream oss;
    oss << "{"
        << "\"type\":\"Circle\","
        << "\"radius\":" << circle.getRadius() << ","
        << "\"x\":" << circle.getPosition().x << ","
        << "\"y\":" << circle.getPosition().y
        << "}";
    return oss.str();
}

// Serialize Rectangle
std::string Serializer::serializeRectangle(const Rectangle& rect) {
    std::ostringstream oss;
    oss << "{"
        << "\"type\":\"Rectangle\","
        << "\"width\":" << rect.getWidth() << ","
        << "\"height\":" << rect.getHeight() << ","
        << "\"x\":" << rect.getPosition().x << ","
        << "\"y\":" << rect.getPosition().y
        << "}";
    return oss.str();
}

// Serialize Text
std::string Serializer::serializeText(const Text& text) {
    std::ostringstream oss;
    oss << "{"
        << "\"type\":\"Text\","
        << "\"content\":\"" << escapeString(text.getContent()) << "\","
        << "\"fontSize\":" << text.getFontSize() << ","
        << "\"x\":" << text.getPosition().x << ","
        << "\"y\":" << text.getPosition().y
        << "}";
    return oss.str();
}

// Serialize any IShape
std::string Serializer::serializeShape(const IShape& shape) {
    switch (shape.getType()) {
    case IShape::ShapeType::Circle:
        return serializeCircle(dynamic_cast<const Circle&>(shape));
    case IShape::ShapeType::Rectangle:
        return serializeRectangle(dynamic_cast<const Rectangle&>(shape));
    case IShape::ShapeType::Text:
        return serializeText(dynamic_cast<const Text&>(shape));
    default:
        return "{}";
    }
}

// Serialize Slide
std::string Serializer::serializeSlide(const Slide& slide) {
    std::ostringstream oss;
    oss << "{"
        << "\"id\":" << slide.getId() << ","
        << "\"shapes\":[";

    for (size_t i = 0; i < slide.getShapeCount(); ++i) {
        if (i > 0) oss << ",";
        const IShape* shape = slide.getShape(i);
        oss << serializeShape(*shape);
    }

    oss << "]}";
    return oss.str();
}

// Serialize Presentation
std::string Serializer::serialize(const Presentation& pres) {
    std::ostringstream oss;
    oss << "{\"slides\":[";

    for (size_t i = 0; i < pres.getSlideCount(); ++i) {
        if (i > 0) oss << ",";
        auto slide = pres.getSlide(i);
        oss << serializeSlide(*slide);
    }

    oss << "]}";
    return oss.str();
}

// Deserialize Circle
std::shared_ptr<Circle> Serializer::deserializeCircle(const std::string& json) {
    float radius = parseFloat(extractValue(json, "radius"));
    float x = parseFloat(extractValue(json, "x"));
    float y = parseFloat(extractValue(json, "y"));
    return std::make_shared<Circle>(radius, x, y);
}

// Deserialize Rectangle
std::shared_ptr<Rectangle> Serializer::deserializeRectangle(const std::string& json) {
    float width = parseFloat(extractValue(json, "width"));
    float height = parseFloat(extractValue(json, "height"));
    float x = parseFloat(extractValue(json, "x"));
    float y = parseFloat(extractValue(json, "y"));
    return std::make_shared<Rectangle>(width, height, x, y);
}

// Deserialize Text
std::shared_ptr<Text> Serializer::deserializeText(const std::string& json) {
    std::string content = extractValue(json, "content");
    float fontSize = parseFloat(extractValue(json, "fontSize"));
    float x = parseFloat(extractValue(json, "x"));
    float y = parseFloat(extractValue(json, "y"));
    return std::make_shared<Text>(content, fontSize, x, y);
}

// Deserialize any Shape
std::shared_ptr<IShape> Serializer::deserializeShape(const std::string& json) {
    std::string type = extractValue(json, "type");

    if (type == "Circle") {
        return deserializeCircle(json);
    }
    else if (type == "Rectangle") {
        return deserializeRectangle(json);
    }
    else if (type == "Text") {
        return deserializeText(json);
    }

    return nullptr;
}

// Deserialize Slide
std::shared_ptr<Slide> Serializer::deserializeSlide(const std::string& json) {
    int id = parseInt(extractValue(json, "id"));
    auto slide = std::make_shared<Slide>(id);

    // Extract shapes array
    std::regex shapesPattern("\"shapes\"\\s*:\\s*\\[([^\\]]*)\\]");
    std::smatch match;
    if (std::regex_search(json, match, shapesPattern)) {
        std::string shapesJson = match[1].str();

        // Simple parsing of shape objects (split by },{ pattern)
        std::regex shapePattern("\\{[^\\}]+\\}");
        std::sregex_iterator iter(shapesJson.begin(), shapesJson.end(), shapePattern);
        std::sregex_iterator end;

        for (; iter != end; ++iter) {
            std::string shapeJson = iter->str();
            auto shape = deserializeShape(shapeJson);
            if (shape) {
                slide->addShape(shape);
            }
        }
    }

    return slide;
}

// Deserialize Presentation
void Serializer::deserialize(Presentation& pres, const std::string& json) {
    // Clear existing slides
    while (pres.getSlideCount() > 0) {
        pres.removeSlide(0);
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

// Save to file - new interface-based version
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

// Load from file - new interface-based version
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

// ============ SVG Export Methods ============

// Helper to escape XML strings
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
    return shape.toSVG();
}

std::string Serializer::slideToSVG(const Slide& slide, size_t slideIndex) {
    std::ostringstream svg;

    // Slide background
    svg << "  <g id=\"slide" << slideIndex << "\">\n";
    svg << "    <rect x=\"0\" y=\"0\" width=\"800\" height=\"600\" "
        << "fill=\"#f0f0f0\" stroke=\"#333\" stroke-width=\"2\"/>\n";

    // Slide title
    svg << "    <text x=\"400\" y=\"30\" font-size=\"24\" "
        << "text-anchor=\"middle\" fill=\"#333\" font-family=\"Arial\">"
        << "Slide " << slideIndex << "</text>\n";

    // Export all shapes
    for (size_t i = 0; i < slide.getShapeCount(); ++i) {
        auto shape = slide.getShape(i);
        if (shape) {
            svg << "    " << shapeToSVG(*shape) << "\n";
        }
    }

    svg << "  </g>\n";
    return svg.str();
}

std::string Serializer::presentationToSVG(const Presentation& pres) {
    std::ostringstream svg;

    // Calculate total height (600 pixels per slide)
    size_t totalHeight = pres.getSlideCount() * 600;
    if (totalHeight == 0) totalHeight = 600; // Minimum height

    // SVG header
    svg << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
        << "width=\"800\" height=\"" << totalHeight
        << "\" viewBox=\"0 0 800 " << totalHeight << "\">\n";

    // Background
    svg << "  <rect width=\"800\" height=\"" << totalHeight
        << "\" fill=\"white\"/>\n";

    // Export each slide
    for (size_t i = 0; i < pres.getSlideCount(); ++i) {
        auto slide = pres.getSlide(i);
        if (slide) {
            // Transform for vertical stacking
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
        // Cast to Presentation
        const Presentation* pres = dynamic_cast<const Presentation*>(&repository);
        if (!pres) {
            std::cerr << "Error: Cannot export non-Presentation repository to SVG" << std::endl;
            return false;
        }

        // Generate SVG
        std::string svgContent = presentationToSVG(*pres);

        // Write to file
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

