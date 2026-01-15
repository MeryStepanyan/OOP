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
    SolidBrush(const std::string& fillColor = "#FFFFFF", float fillOpacity = 1.0f);

    // IPaintProperties implementation
    std::string getColor() const override;
    void setColor(const std::string& c) override;

    float getOpacity() const override;
    void setOpacity(float o) override;

    bool isVisible() const override;
    void setVisible(bool v) override;

    // IBrush implementation
    BrushType getType() const override;
    void setType(BrushType t) override;

    std::string getGradientStartColor() const override;
    void setGradientStartColor(const std::string& c) override;

    std::string getGradientEndColor() const override;
    void setGradientEndColor(const std::string& c) override;

    std::string getPatternId() const override;
    void setPatternId(const std::string& id) override;

    // Helper method for SVG export
    std::string toSVGFill() const;
};