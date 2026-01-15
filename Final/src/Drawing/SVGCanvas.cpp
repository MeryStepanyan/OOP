
#include "SVGCanvas.h"


    SVGCanvas::SVGCanvas(float w , float h )
        : width(w), height(h), inPath(false) {
        currentPen = std::make_shared<Pen>();
        currentBrush = std::make_shared<SolidBrush>();
        currentTransform = TransformState();
    }


    // Canvas dimensions
    float SVGCanvas::getWidth() const  { return width; }
    float SVGCanvas::getHeight() const  { return height; }
    void SVGCanvas::setSize(float w, float h)  {
        width = w;
        height = h;
    }

    // Clear canvas
    void SVGCanvas::clear()  {
        svgContent.str("");
        svgContent.clear();
        pathContent.str("");
        pathContent.clear();
        inPath = false;
    }

    void SVGCanvas::clear(const std::string& backgroundColor)  {
        clear();
        svgContent << "<rect x=\"0\" y=\"0\" width=\"" << width
            << "\" height=\"" << height
            << "\" fill=\"" << backgroundColor << "\"/>\n";
    }

    // Drawing primitives
    void SVGCanvas::drawLine(float x1, float y1, float x2, float y2)  {
        auto pen = std::dynamic_pointer_cast<Pen>(currentPen);
        if (!pen || !pen->isVisible()) return;

        // Apply current transform to coordinates
        float tx1 = x1 + currentTransform.translateX;
        float ty1 = y1 + currentTransform.translateY;
        float tx2 = x2 + currentTransform.translateX;
        float ty2 = y2 + currentTransform.translateY;

        svgContent << "<line x1=\"" << tx1 << "\" y1=\"" << ty1
            << "\" x2=\"" << tx2 << "\" y2=\"" << ty2 << "\" "
            << pen->toSVGStroke() << "/>\n";
    }

    void SVGCanvas::drawRectangle(float x, float y, float w, float h)  {
        auto pen = std::dynamic_pointer_cast<Pen>(currentPen);
        auto brush = std::dynamic_pointer_cast<SolidBrush>(currentBrush);

        // Apply current transform
        float tx = x + currentTransform.translateX;
        float ty = y + currentTransform.translateY;

        svgContent << "<rect x=\"" << tx << "\" y=\"" << ty
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

        svgContent << "/>\n";
    }

    void SVGCanvas::drawCircle(float centerX, float centerY, float radius)  {
        auto pen = std::dynamic_pointer_cast<Pen>(currentPen);
        auto brush = std::dynamic_pointer_cast<SolidBrush>(currentBrush);

        // Apply current transform
        float tx = centerX + currentTransform.translateX;
        float ty = centerY + currentTransform.translateY;

        svgContent << "<circle cx=\"" << tx << "\" cy=\"" << ty
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

        svgContent << "/>\n";
    }

    void SVGCanvas::drawEllipse(float centerX, float centerY, float radiusX, float radiusY)  {
        auto pen = std::dynamic_pointer_cast<Pen>(currentPen);
        auto brush = std::dynamic_pointer_cast<SolidBrush>(currentBrush);

        // Apply current transform
        float tx = centerX + currentTransform.translateX;
        float ty = centerY + currentTransform.translateY;

        svgContent << "<ellipse cx=\"" << tx << "\" cy=\"" << ty
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

        svgContent << "/>\n";
    }

    void SVGCanvas::drawText(float x, float y, const std::string& text, float fontSize) {
        auto pen = std::dynamic_pointer_cast<Pen>(currentPen);

        // Apply current transform
        float tx = x + currentTransform.translateX;
        float ty = y + currentTransform.translateY;

        // CRITICAL FIX for TEXT: Use proper baseline alignment
        svgContent << "<text x=\"" << tx << "\" y=\"" << ty
            << "\" font-size=\"" << fontSize << "\" "
            << "font-family=\"Arial, sans-serif\" "
            << "dominant-baseline=\"hanging\" ";  // FIX: Proper baseline

        if (pen && pen->isVisible()) {
            svgContent << "fill=\"" << pen->getColor() << "\" ";
        }
        else {
            svgContent << "fill=\"#000000\" ";  // Default black for text
        }

        svgContent << ">"
            << escapeXML(text) << "</text>\n";
    }

    void SVGCanvas::drawPolygon(const std::vector<Point>& points)  {
        if (points.empty()) return;

        auto pen = std::dynamic_pointer_cast<Pen>(currentPen);
        auto brush = std::dynamic_pointer_cast<SolidBrush>(currentBrush);

        svgContent << "<polygon points=\"";
        for (const auto& p : points) {
            // Apply current transform
            float tx = p.x + currentTransform.translateX;
            float ty = p.y + currentTransform.translateY;
            svgContent << tx << "," << ty << " ";
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

        svgContent << "/>\n";
    }

    // Path operations
    void SVGCanvas::beginPath()  {
        pathContent.str("");
        pathContent.clear();
        inPath = true;
    }

    void SVGCanvas::moveTo(float x, float y)  {
        if (inPath) {
            float tx = x + currentTransform.translateX;
            float ty = y + currentTransform.translateY;
            pathContent << "M " << tx << " " << ty << " ";
        }
    }

    void SVGCanvas::lineTo(float x, float y)  {
        if (inPath) {
            float tx = x + currentTransform.translateX;
            float ty = y + currentTransform.translateY;
            pathContent << "L " << tx << " " << ty << " ";
        }
    }

    void SVGCanvas::arcTo(float x1, float y1, float x2, float y2, float radius)  {
        if (inPath) {
            float tx1 = x1 + currentTransform.translateX;
            float ty1 = y1 + currentTransform.translateY;
            float tx2 = x2 + currentTransform.translateX;
            float ty2 = y2 + currentTransform.translateY;
            pathContent << "Q " << tx1 << " " << ty1 << " " << tx2 << " " << ty2 << " ";
        }
    }

    void SVGCanvas::closePath()  {
        if (inPath) {
            pathContent << "Z ";
        }
    }

    void SVGCanvas::stroke()  {
        if (inPath) {
            auto pen = std::dynamic_pointer_cast<Pen>(currentPen);
            svgContent << "<path d=\"" << pathContent.str() << "\" fill=\"none\" ";
            if (pen && pen->isVisible()) {
                svgContent << pen->toSVGStroke();
            }
            svgContent << "/>\n";
            inPath = false;
        }
    }

    void SVGCanvas::fill()  {
        if (inPath) {
            auto brush = std::dynamic_pointer_cast<SolidBrush>(currentBrush);
            svgContent << "<path d=\"" << pathContent.str() << "\" ";
            if (brush && brush->isVisible()) {
                svgContent << brush->toSVGFill();
            }
            svgContent << " stroke=\"none\"/>\n";
            inPath = false;
        }
    }

    // CRITICAL FIX: Transform operations with proper save/restore
    void SVGCanvas::save()  {
        // Push COPY of current transform onto stack
        transformStack.push(currentTransform);
    }

    void SVGCanvas::restore()  {
        if (!transformStack.empty()) {
            // Restore previous transform state
            currentTransform = transformStack.top();
            transformStack.pop();
        }
    }

    void SVGCanvas::translate(float dx, float dy)  {
        currentTransform.translateX += dx;
        currentTransform.translateY += dy;
    }

    void SVGCanvas::rotate(float angle)  {
        currentTransform.rotation += angle;
    }

    void SVGCanvas::scale(float sx, float sy)  {
        currentTransform.scaleX *= sx;
        currentTransform.scaleY *= sy;
    }

    // Set drawing properties
    void SVGCanvas::setPen(std::shared_ptr<IPen> pen)  {
        currentPen = pen;
    }

    void SVGCanvas::setBrush(std::shared_ptr<IBrush> brush)  {
        currentBrush = brush;
    }

    std::shared_ptr<IPen> SVGCanvas::getPen() const  {
        return currentPen;
    }

    std::shared_ptr<IBrush> SVGCanvas::getBrush() const  {
        return currentBrush;
    }

    // Clipping (basic implementation)
    void SVGCanvas::setClipRect(float x, float y, float w, float h)  {
        svgContent << "<clipPath id=\"clip\">\n";
        svgContent << "  <rect x=\"" << x << "\" y=\"" << y
            << "\" width=\"" << w << "\" height=\"" << h << "\"/>\n";
        svgContent << "</clipPath>\n";
    }

    void SVGCanvas::clearClip() {
        // No-op for now
    }

    // Export/Save
    bool SVGCanvas::saveToFile(const std::string& filename) const  {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        file << exportToSVG();
        file.close();
        return true;
    }

    std::string SVGCanvas::exportToSVG() const  {
        std::ostringstream output;
        output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        output << "<svg width=\"" << width << "\" height=\"" << height
            << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
        output << svgContent.str();
        output << "</svg>";
        return output.str();
    }

    std::string SVGCanvas::escapeXML(const std::string& str) const {
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
