#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <memory>

// Forward declarations
class ISerializableRepository;
class Presentation;
class Slide;
class IShape;
class StyledShape;  // NEW: Added
class ShapeStyle;   // NEW: Added
class Circle;
class Rectangle;
class Text;
class Image;
class Line;
class Triangle;
class Ellipse;
class Square;
class Polygon;

/**
 * Serializer - UPDATED VERSION with Style Support
 *
 * CHANGES:
 * 1. ✅ Now serializes StyledShape (includes style)
 * 2. ✅ Added style serialization methods
 * 3. ✅ Better error handling
 * 4. ✅ Consistent JSON format
 */
class Serializer {
public:
    // ========================================================================
    // FILE I/O
    // ========================================================================
    static bool saveToFile(const ISerializableRepository& repository, const std::string& filename);
    static bool loadFromFile(ISerializableRepository& repository, const std::string& filename);

    // ========================================================================
    // PRESENTATION SERIALIZATION
    // ========================================================================
    static std::string serialize(const Presentation& pres);
    static void deserialize(Presentation& pres, const std::string& json);

    // ========================================================================
    // SVG EXPORT
    // ========================================================================
    static bool exportToSVG(const ISerializableRepository& repository, const std::string& filename);
    static std::string presentationToSVG(const Presentation& pres);
    static std::string slideToSVG(const Slide& slide, size_t slideIndex = 0);
    static std::string shapeToSVG(const IShape& shape);

private:
    // ========================================================================
    // SLIDE SERIALIZATION
    // ========================================================================
    static std::string serializeSlide(const Slide& slide);
    static std::shared_ptr<Slide> deserializeSlide(const std::string& json);

    // ========================================================================
    // STYLED SHAPE SERIALIZATION (NEW)
    // ========================================================================
    static std::string serializeStyledShape(const StyledShape& styledShape);
    static StyledShape deserializeStyledShape(const std::string& json);

    // ========================================================================
    // SHAPE SERIALIZATION (Deprecated - use serializeStyledShape)
    // ========================================================================
    static std::string serializeShape(const IShape& shape);  // Legacy
    static std::shared_ptr<IShape> deserializeShape(const std::string& json);  // Legacy

    // Shape-specific serialization (geometry only)
    static std::string serializeCircle(const Circle& circle);
    static std::string serializeRectangle(const Rectangle& rect);
    static std::string serializeText(const Text& text);
    static std::string serializeImage(const Image& image);
    static std::string serializeLine(const Line& line);
    static std::string serializeTriangle(const Triangle& triangle);
    static std::string serializeEllipse(const Ellipse& ellipse);
    static std::string serializeSquare(const Square& square);
    static std::string serializePolygon(const Polygon& polygon);

    // Shape-specific deserialization
    static std::shared_ptr<Circle> deserializeCircle(const std::string& json);
    static std::shared_ptr<Rectangle> deserializeRectangle(const std::string& json);
    static std::shared_ptr<Text> deserializeText(const std::string& json);
    static std::shared_ptr<Image> deserializeImage(const std::string& json);
    static std::shared_ptr<Line> deserializeLine(const std::string& json);
    static std::shared_ptr<Triangle> deserializeTriangle(const std::string& json);
    static std::shared_ptr<Ellipse> deserializeEllipse(const std::string& json);
    static std::shared_ptr<Square> deserializeSquare(const std::string& json);
    static std::shared_ptr<Polygon> deserializePolygon(const std::string& json);

    // ========================================================================
    // STYLE SERIALIZATION (NEW)
    // ========================================================================
    static std::string serializeStyle(const ShapeStyle& style);
    static ShapeStyle deserializeStyle(const std::string& json);

    // ========================================================================
    // UTILITY METHODS
    // ========================================================================
    static std::string escapeString(const std::string& str);
    static std::string unescapeString(const std::string& str);
    static std::string extractValue(const std::string& json, const std::string& key);
    static float parseFloat(const std::string& str);
    static int parseInt(const std::string& str);

    // NEW: Extract JSON object
    static std::string extractObject(const std::string& json, const std::string& key);
};