#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "ICanvas.h"
#include "IPen.h"
#include "IBrush.h"
#include "Pen.h"
#include "SolidBrush.h"
#include <sstream>
#include <fstream>
#include <stack>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class SVGCanvas : public ICanvas {
private:
    float width;
    float height;
    std::shared_ptr<IPen> currentPen;
    std::shared_ptr<IBrush> currentBrush;
    std::ostringstream svgContent;
    std::ostringstream pathContent;
    bool inPath;

    // Transform state
    struct TransformState {
        float translateX = 0;
        float translateY = 0;
        float scaleX = 1;
        float scaleY = 1;
        float rotation = 0;

        // Copy constructor for proper stack usage
        TransformState() = default;
        TransformState(const TransformState& other) = default;
        TransformState& operator=(const TransformState& other) = default;
    };

    std::stack<TransformState> transformStack;
    TransformState currentTransform;

public:
    SVGCanvas(float w = 800.0f, float h = 600.0f);


    // Canvas dimensions
    float getWidth() const override;
    float getHeight() const override;
    void setSize(float w, float h) override;

    // Clear canvas
    void clear() override;

    void clear(const std::string& backgroundColor) override;

    // Drawing primitives
    void drawLine(float x1, float y1, float x2, float y2) override;

    void drawRectangle(float x, float y, float w, float h) override;

    void drawCircle(float centerX, float centerY, float radius) override;

    void drawEllipse(float centerX, float centerY, float radiusX, float radiusY) override;

    void drawText(float x, float y, const std::string& text, float fontSize) override;

    void drawPolygon(const std::vector<Point>& points) override;
    // Path operations
    void beginPath() override;
    void moveTo(float x, float y) override;

    void lineTo(float x, float y) override;

    void arcTo(float x1, float y1, float x2, float y2, float radius) override;

    void closePath() override;
    void stroke() override;

    void fill() override;

    // CRITICAL FIX: Transform operations with proper save/restore
    void save() override;

    void restore() override;

    void translate(float dx, float dy) override;
    void rotate(float angle) override;

    void scale(float sx, float sy) override;


    // Set drawing properties
    void setPen(std::shared_ptr<IPen> pen) override;

    void setBrush(std::shared_ptr<IBrush> brush) override;

    std::shared_ptr<IPen> getPen() const override;

    std::shared_ptr<IBrush> getBrush() const override;

    // Clipping (basic implementation)
    void setClipRect(float x, float y, float w, float h) override; 

    void clearClip() override;

    // Export/Save
    bool saveToFile(const std::string& filename) const override;

    std::string exportToSVG() const override;

private:
    std::string escapeXML(const std::string& str) const;
};