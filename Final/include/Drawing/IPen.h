#pragma once
#include "IPaintProperties.h"
#include <memory>

// Pen interface for stroke/outline operations
class IPen : public IPaintProperties {
public:
    virtual ~IPen() = default;

    // Stroke width
    virtual float getWidth() const = 0;
    virtual void setWidth(float width) = 0;

    // Line style
    enum class LineStyle {
        Solid,
        Dashed,
        Dotted,
        DashDot
    };

    virtual LineStyle getStyle() const = 0;
    virtual void setStyle(LineStyle style) = 0;

    // Line cap style
    enum class LineCap {
        Flat,
        Round,
        Square
    };

    virtual LineCap getLineCap() const = 0;
    virtual void setLineCap(LineCap cap) = 0;

    // Line join style
    enum class LineJoin {
        Miter,
        Round,
        Bevel
    };

    virtual LineJoin getLineJoin() const = 0;
    virtual void setLineJoin(LineJoin join) = 0;
};
#pragma once
