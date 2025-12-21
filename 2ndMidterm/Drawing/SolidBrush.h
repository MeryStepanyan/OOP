#pragma once
#include "IBrush.h"

class SolidBrush : public IBrush {
private:
    std::string color;
    float opacity;
    bool visible;
    BrushType type;
    std::string gradientStartColor;
    std::string gradientEndColor;
    std::string patternId;

public:
    SolidBrush(const std::string& fillColor = "#FFFFFF", float fillOpacity = 1.0f)
        : color(fillColor),
        opacity(fillOpacity),
        visible(true),
        type(BrushType::Solid),
        gradientStartColor(""),
        gradientEndColor(""),
        patternId("") {
    }

    // IPaintProperties implementation
    std::string getColor() const override { return color; }
    void setColor(const std::string& c) override { color = c; }

    float getOpacity() const override { return opacity; }
    void setOpacity(float o) override { opacity = o; }

    bool isVisible() const override { return visible; }
    void setVisible(bool v) override { visible = v; }

    // IBrush implementation
    BrushType getType() const override { return type; }
    void setType(BrushType t) override { type = t; }

    std::string getGradientStartColor() const override { return gradientStartColor; }
    void setGradientStartColor(const std::string& c) override { gradientStartColor = c; }

    std::string getGradientEndColor() const override { return gradientEndColor; }
    void setGradientEndColor(const std::string& c) override { gradientEndColor = c; }

    std::string getPatternId() const override { return patternId; }
    void setPatternId(const std::string& id) override { patternId = id; }

    // Helper method for SVG export
    std::string toSVGFill() const {
        if (!visible || type == BrushType::None) {
            return "fill=\"none\"";
        }

        std::string result = "fill=\"" + color + "\"";
        if (opacity < 1.0f) {
            result += " fill-opacity=\"" + std::to_string(opacity) + "\"";
        }
        return result;
    }
};