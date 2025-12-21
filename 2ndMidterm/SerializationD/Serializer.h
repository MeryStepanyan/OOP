
#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <memory>


class ISerializableRepository;
class Presentation;
class Slide;
class IShape;
class Circle;
class Rectangle;
class Text;

class Serializer {
public:

    static bool saveToFile(const ISerializableRepository& repository, const std::string& filename);


    static bool loadFromFile(ISerializableRepository& repository, const std::string& filename);

    static std::string serialize(const Presentation& pres);

    static void deserialize(Presentation& pres, const std::string& json);

    // SVG export methods
    static bool exportToSVG(const ISerializableRepository& repository, const std::string& filename);
    static std::string presentationToSVG(const Presentation& pres);
    static std::string slideToSVG(const Slide& slide, size_t slideIndex = 0);
    static std::string shapeToSVG(const IShape& shape);

private:

    static std::string serializeSlide(const Slide& slide);
    static std::string serializeShape(const IShape& shape);
    static std::string serializeCircle(const Circle& circle);
    static std::string serializeRectangle(const Rectangle& rect);
    static std::string serializeText(const Text& text);


    static std::shared_ptr<Slide> deserializeSlide(const std::string& json);
    static std::shared_ptr<IShape> deserializeShape(const std::string& json);
    static std::shared_ptr<Circle> deserializeCircle(const std::string& json);
    static std::shared_ptr<Rectangle> deserializeRectangle(const std::string& json);
    static std::shared_ptr<Text> deserializeText(const std::string& json);


    static std::string escapeString(const std::string& str);
    static std::string unescapeString(const std::string& str);
    static std::string extractValue(const std::string& json, const std::string& key);
    static float parseFloat(const std::string& str);
    static int parseInt(const std::string& str);
};

