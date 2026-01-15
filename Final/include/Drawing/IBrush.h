#pragma once
#include "IPaintProperties.h"

// Brush interface for fill operations
class IBrush : public IPaintProperties {
public:
    virtual ~IBrush() = default;

    // Brush type
    enum class BrushType {
        Solid,      // Solid color fill
        Gradient,   // Gradient fill
        Pattern,    // Pattern/texture fill
        None        // No fill
    };

    virtual BrushType getType() const = 0;
    virtual void setType(BrushType type) = 0;

    // For gradient brushes
    virtual std::string getGradientStartColor() const = 0;
    virtual void setGradientStartColor(const std::string& color) = 0;

    virtual std::string getGradientEndColor() const = 0;
    virtual void setGradientEndColor(const std::string& color) = 0;

    // For pattern brushes
    virtual std::string getPatternId() const = 0;
    virtual void setPatternId(const std::string& patternId) = 0;
};
