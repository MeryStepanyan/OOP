
#include "Pen.h"


Pen::Pen(const std::string& strokeColor  ,
        float strokeWidth ,
        float strokeOpacity )
        : color(strokeColor),
        opacity(strokeOpacity),
        visible(true),
        width(strokeWidth),
        style(LineStyle::Solid),
        lineCap(LineCap::Flat),
        lineJoin(LineJoin::Miter) {
    }

    // IPaintProperties implementation
    std::string Pen::getColor() const  { return color; }
    void Pen::setColor(const std::string& c)  { color = c; }

    float Pen::getOpacity() const  { return opacity; }
    void Pen::setOpacity(float o)  { opacity = o; }

    bool Pen::isVisible() const  { return visible; }
    void Pen::setVisible(bool v)  { visible = v; }

    // IPen implementation
    float Pen::getWidth() const  { return width; }
    void Pen::setWidth(float w)  { width = w; }

    Pen::LineStyle Pen::getStyle() const  { return style; }
    void Pen::setStyle(LineStyle s)  { style = s; }

    Pen::LineCap Pen::getLineCap() const  { return lineCap; }
    void Pen::setLineCap(LineCap cap)  { lineCap = cap; }

    Pen::LineJoin Pen::getLineJoin() const  { return lineJoin; }
    void Pen::setLineJoin(LineJoin join)  { lineJoin = join; }

    // Helper method for SVG export
    std::string Pen::toSVGStroke() const {
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
