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
        float strokeOpacity = 1.0f);

    // IPaintProperties implementation
    std::string getColor() const override;
    void setColor(const std::string& c) override;

    float getOpacity() const override;

    void setOpacity(float o) override;

    bool isVisible() const override;
    void setVisible(bool v) override;

    // IPen implementation
    float getWidth() const override;
    void setWidth(float w) override;

    LineStyle getStyle() const override;
    void setStyle(LineStyle s) override;

    LineCap getLineCap() const override;
    void setLineCap(LineCap cap) override;

    LineJoin getLineJoin() const override;
    void setLineJoin(LineJoin join) override;

    // Helper method for SVG export
    std::string toSVGStroke() const;
};