#pragma once
#include "IPen.h"

class Pen : public IPen {
private:
    std::string color;
    float opacity;
    bool visible;
    float width;
    LineStyle style;
    LineCap lineCap;
    LineJoin lineJoin;

public:
    Pen(const std::string& strokeColor = "#000000",
        float strokeWidth = 1.0f,
        float strokeOpacity = 1.0f)
        : color(strokeColor),
        opacity(strokeOpacity),
        visible(true),
        width(strokeWidth),
        style(LineStyle::Solid),
        lineCap(LineCap::Flat),
        lineJoin(LineJoin::Miter) {
    }

    // IPaintProperties implementation
    std::string getColor() const override { return color; }
    void setColor(const std::string& c) override { color = c; }

    float getOpacity() const override { return opacity; }
    void setOpacity(float o) override { opacity = o; }

    bool isVisible() const override { return visible; }
    void setVisible(bool v) override { visible = v; }

    // IPen implementation
    float getWidth() const override { return width; }
    void setWidth(float w) override { width = w; }

    LineStyle getStyle() const override { return style; }
    void setStyle(LineStyle s) override { style = s; }

    LineCap getLineCap() const override { return lineCap; }
    void setLineCap(LineCap cap) override { lineCap = cap; }

    LineJoin getLineJoin() const override { return lineJoin; }
    void setLineJoin(LineJoin join) override { lineJoin = join; }

    // Helper method for SVG export
    std::string toSVGStroke() const {
        if (!visible) {
            return "stroke=\"none\"";
        }

        std::string result = "stroke=\"" + color + "\" ";
        result += "stroke-width=\"" + std::to_string(width) + "\" ";

        if (opacity < 1.0f) {
            result += "stroke-opacity=\"" + std::to_string(opacity) + "\" ";
        }

        // Line style
        switch (style) {
        case LineStyle::Dashed:
            result += "stroke-dasharray=\"5,5\" ";
            break;
        case LineStyle::Dotted:
            result += "stroke-dasharray=\"2,2\" ";
            break;
        case LineStyle::DashDot:
            result += "stroke-dasharray=\"5,2,2,2\" ";
            break;
        default:
            break;
        }

        // Line cap
        switch (lineCap) {
        case LineCap::Round:
            result += "stroke-linecap=\"round\" ";
            break;
        case LineCap::Square:
            result += "stroke-linecap=\"square\" ";
            break;
        default:
            result += "stroke-linecap=\"butt\" ";
            break;
        }

        // Line join
        switch (lineJoin) {
        case LineJoin::Round:
            result += "stroke-linejoin=\"round\" ";
            break;
        case LineJoin::Bevel:
            result += "stroke-linejoin=\"bevel\" ";
            break;
        default:
            result += "stroke-linejoin=\"miter\" ";
            break;
        }

        return result;
    }
};