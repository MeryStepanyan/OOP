
#include "SolidBrush.h"

    SolidBrush::SolidBrush(const std::string& fillColor , float fillOpacity )
        : color(fillColor),
        opacity(fillOpacity),
        visible(true),
        type(BrushType::Solid),
        gradientStartColor(""),
        gradientEndColor(""),
        patternId("") {
    }

    // IPaintProperties implementation
    std::string SolidBrush::getColor() const  { return color; }
    void SolidBrush::setColor(const std::string& c) { color = c; }

    float SolidBrush::getOpacity() const  { return opacity; }
    void SolidBrush::setOpacity(float o)  { opacity = o; }

    bool SolidBrush::isVisible() const  { return visible; }
    void SolidBrush::setVisible(bool v)  { visible = v; }

    // IBrush implementation
    SolidBrush::BrushType SolidBrush::getType() const  { return type; }
    void SolidBrush::setType(BrushType t)  { type = t; }

    std::string SolidBrush::getGradientStartColor() const  { return gradientStartColor; }
    void SolidBrush::setGradientStartColor(const std::string& c) { gradientStartColor = c; }

    std::string SolidBrush::getGradientEndColor() const  { return gradientEndColor; }
    void SolidBrush::setGradientEndColor(const std::string& c)  { gradientEndColor = c; }

    std::string SolidBrush::getPatternId() const { return patternId; }
    void SolidBrush::setPatternId(const std::string& id)  { patternId = id; }

    // Helper method for SVG export
    std::string SolidBrush::toSVGFill() const {
        if (!visible || type == BrushType::None) {
            return "fill=\"none\"";
        }

        std::string result = "fill=\"" + color + "\"";
        if (opacity < 1.0f) {
            result += " fill-opacity=\"" + std::to_string(opacity) + "\"";
        }
        return result;
    }
