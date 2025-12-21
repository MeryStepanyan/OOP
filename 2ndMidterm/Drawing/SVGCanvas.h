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
    };
    std::stack<TransformState> transformStack;
    TransformState currentTransform;

public:
    SVGCanvas(float w = 800.0f, float h = 600.0f)
        : width(w), height(h), inPath(false) {
        currentPen = std::make_shared<Pen>();
        currentBrush = std::make_shared<SolidBrush>();
        currentTransform = TransformState();
    }

    // Canvas dimensions
    float getWidth() const override { return width; }
    float getHeight() const override { return height; }
    void setSize(float w, float h) override {
        width = w;
        height = h;
    }

    // Clear canvas
    void clear() override {
        svgContent.str("");
        svgContent.clear();
        pathContent.str("");
        pathContent.clear();
        inPath = false;
    }

    void clear(const std::string& backgroundColor) override {
        clear();
        svgContent << "<rect x=\"0\" y=\"0\" width=\"" << width
            << "\" height=\"" << height
            << "\" fill=\"" << backgroundColor << "\"/>\n";
    }

    // Drawing primitives
    void drawLine(float x1, float y1, float x2, float y2) override {
        auto pen = std::dynamic_pointer_cast<Pen>(currentPen);
        if (!pen || !pen->isVisible()) return;

        svgContent << "<line x1=\"" << x1 << "\" y1=\"" << y1
            << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" "
            << getTransformAttr()
            << pen->toSVGStroke() << "/>\n";
    }

    void drawRectangle(float x, float y, float w, float h) override {
        auto pen = std::dynamic_pointer_cast<Pen>(currentPen);
        auto brush = std::dynamic_pointer_cast<SolidBrush>(currentBrush);

        svgContent << "<rect x=\"" << x << "\" y=\"" << y
            << "\" width=\"" << w << "\" height=\"" << h << "\" ";

        if (brush && brush->isVisible()) {
            svgContent << brush->toSVGFill() << " ";
        }
        else {
            svgContent << "fill=\"none\" ";
        }

        if (pen && pen->isVisible()) {
            svgContent << pen->toSVGStroke();
        }

        svgContent << getTransformAttr() << "/>\n";
    }

    void drawCircle(float centerX, float centerY, float radius) override {
        auto pen = std::dynamic_pointer_cast<Pen>(currentPen);
        auto brush = std::dynamic_pointer_cast<SolidBrush>(currentBrush);

        svgContent << "<circle cx=\"" << centerX << "\" cy=\"" << centerY
            << "\" r=\"" << radius << "\" ";

        if (brush && brush->isVisible()) {
            svgContent << brush->toSVGFill() << " ";
        }
        else {
            svgContent << "fill=\"none\" ";
        }

        if (pen && pen->isVisible()) {
            svgContent << pen->toSVGStroke();
        }

        svgContent << getTransformAttr() << "/>\n";
    }

    void drawEllipse(float centerX, float centerY, float radiusX, float radiusY) override {
        auto pen = std::dynamic_pointer_cast<Pen>(currentPen);
        auto brush = std::dynamic_pointer_cast<SolidBrush>(currentBrush);

        svgContent << "<ellipse cx=\"" << centerX << "\" cy=\"" << centerY
            << "\" rx=\"" << radiusX << "\" ry=\"" << radiusY << "\" ";

        if (brush && brush->isVisible()) {
            svgContent << brush->toSVGFill() << " ";
        }
        else {
            svgContent << "fill=\"none\" ";
        }

        if (pen && pen->isVisible()) {
            svgContent << pen->toSVGStroke();
        }

        svgContent << getTransformAttr() << "/>\n";
    }

    void drawText(float x, float y, const std::string& text, float fontSize) override {
        auto pen = std::dynamic_pointer_cast<Pen>(currentPen);

        svgContent << "<text x=\"" << x << "\" y=\"" << y
            << "\" font-size=\"" << fontSize << "\" ";

        if (pen && pen->isVisible()) {
            svgContent << "fill=\"" << pen->getColor() << "\" ";
        }

        svgContent << getTransformAttr() << ">"
            << escapeXML(text) << "</text>\n";
    }

    void drawPolygon(const std::vector<Point>& points) override {
        if (points.empty()) return;

        auto pen = std::dynamic_pointer_cast<Pen>(currentPen);
        auto brush = std::dynamic_pointer_cast<SolidBrush>(currentBrush);

        svgContent << "<polygon points=\"";
        for (const auto& p : points) {
            svgContent << p.x << "," << p.y << " ";
        }
        svgContent << "\" ";

        if (brush && brush->isVisible()) {
            svgContent << brush->toSVGFill() << " ";
        }
        else {
            svgContent << "fill=\"none\" ";
        }

        if (pen && pen->isVisible()) {
            svgContent << pen->toSVGStroke();
        }

        svgContent << getTransformAttr() << "/>\n";
    }

    // Path operations
    void beginPath() override {
        pathContent.str("");
        pathContent.clear();
        inPath = true;
    }

    void moveTo(float x, float y) override {
        if (inPath) {
            pathContent << "M " << x << " " << y << " ";
        }
    }

    void lineTo(float x, float y) override {
        if (inPath) {
            pathContent << "L " << x << " " << y << " ";
        }
    }

    void arcTo(float x1, float y1, float x2, float y2, float radius) override {
        if (inPath) {
            // Simplified arc implementation
            pathContent << "Q " << x1 << " " << y1 << " " << x2 << " " << y2 << " ";
        }
    }

    void closePath() override {
        if (inPath) {
            pathContent << "Z ";
        }
    }

    void stroke() override {
        if (inPath) {
            auto pen = std::dynamic_pointer_cast<Pen>(currentPen);
            svgContent << "<path d=\"" << pathContent.str() << "\" fill=\"none\" ";
            if (pen && pen->isVisible()) {
                svgContent << pen->toSVGStroke();
            }
            svgContent << getTransformAttr() << "/>\n";
            inPath = false;
        }
    }

    void fill() override {
        if (inPath) {
            auto brush = std::dynamic_pointer_cast<SolidBrush>(currentBrush);
            svgContent << "<path d=\"" << pathContent.str() << "\" ";
            if (brush && brush->isVisible()) {
                svgContent << brush->toSVGFill();
            }
            svgContent << " stroke=\"none\" " << getTransformAttr() << "/>\n";
            inPath = false;
        }
    }

    // Transform operations
    void save() override {
        transformStack.push(currentTransform);
    }

    void restore() override {
        if (!transformStack.empty()) {
            currentTransform = transformStack.top();
            transformStack.pop();
        }
    }

    void translate(float dx, float dy) override {
        currentTransform.translateX += dx;
        currentTransform.translateY += dy;
    }

    void rotate(float angle) override {
        currentTransform.rotation += angle;
    }

    void scale(float sx, float sy) override {
        currentTransform.scaleX *= sx;
        currentTransform.scaleY *= sy;
    }

    // Set drawing properties
    void setPen(std::shared_ptr<IPen> pen) override {
        currentPen = pen;
    }

    void setBrush(std::shared_ptr<IBrush> brush) override {
        currentBrush = brush;
    }

    std::shared_ptr<IPen> getPen() const override {
        return currentPen;
    }

    std::shared_ptr<IBrush> getBrush() const override {
        return currentBrush;
    }

    // Clipping (basic implementation)
    void setClipRect(float x, float y, float w, float h) override {
        svgContent << "<clipPath id=\"clip\">\n";
        svgContent << "  <rect x=\"" << x << "\" y=\"" << y
            << "\" width=\"" << w << "\" height=\"" << h << "\"/>\n";
        svgContent << "</clipPath>\n";
    }

    void clearClip() override {
        // No-op for now
    }

    // Export/Save
    bool saveToFile(const std::string& filename) const override {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        file << exportToSVG();
        file.close();
        return true;
    }

    std::string exportToSVG() const override {
        std::ostringstream output;
        output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        output << "<svg width=\"" << width << "\" height=\"" << height
            << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
        output << svgContent.str();
        output << "</svg>";
        return output.str();
    }

private:
    std::string getTransformAttr() const {
        if (currentTransform.translateX == 0 && currentTransform.translateY == 0 &&
            currentTransform.scaleX == 1 && currentTransform.scaleY == 1 &&
            currentTransform.rotation == 0) {
            return "";
        }

        std::string result = "transform=\"";

        if (currentTransform.translateX != 0 || currentTransform.translateY != 0) {
            result += "translate(" + std::to_string(currentTransform.translateX) + ","
                + std::to_string(currentTransform.translateY) + ") ";
        }

        if (currentTransform.rotation != 0) {
            result += "rotate(" + std::to_string(currentTransform.rotation * 180.0f / M_PI) + ") ";
        }

        if (currentTransform.scaleX != 1 || currentTransform.scaleY != 1) {
            result += "scale(" + std::to_string(currentTransform.scaleX) + ","
                + std::to_string(currentTransform.scaleY) + ")";
        }

        result += "\" ";
        return result;
    }

    std::string escapeXML(const std::string& str) const {
        std::string result;
        for (char c : str) {
            switch (c) {
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '&': result += "&amp;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&apos;"; break;
            default: result += c; break;
            }
        }
        return result;
    }
};