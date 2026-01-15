
#include "RenderingContext.h"




    RenderingContext::RenderingContext(std::shared_ptr<ICanvas> c,
        std::shared_ptr<IShapeVisitor> r)
        : canvas(c), renderer(r) {
    }


    // CANVAS SETUP


    void RenderingContext::clear() {
        canvas->clear();
    }

    void RenderingContext::setBackground(const std::string& color) {
        canvas->clear(color);
    }

    void RenderingContext::setSize(float width, float height) {
        canvas->setSize(width, height);
    }


    // RENDERING - Single Shape



    void RenderingContext::render(const StyledShape& styledShape) {
        if (!styledShape.isValid()) {
            return;  // Skip invalid shapes
        }

        // Step 1: Get style
        const auto& style = styledShape.getStyle();

        // Step 2: Apply pen (outline/stroke)
        if (style.hasPen()) {
            canvas->setPen(style.getPen());
        }

        // Step 3: Apply brush (fill)
        if (style.hasBrush()) {
            canvas->setBrush(style.getBrush());
        }

        // Step 4: Render using visitor pattern
        // This calls renderer->visit(shape) based on actual type
        styledShape.accept(*renderer);
    }

    // RENDERING - Multiple Shapes



    void RenderingContext::render(const std::vector<StyledShape>& shapes) {
        for (const auto& shape : shapes) {
            render(shape);
        }
    }


    void RenderingContext::renderShapes(const std::vector<std::shared_ptr<StyledShape>>& shapes) {
        for (const auto& shape : shapes) {
            if (shape) {
                render(*shape);
            }
        }
    }


    // ADVANCED RENDERING



    void RenderingContext::renderWithStyle(const StyledShape& styledShape,
        const ShapeStyle& customStyle) {
        // Apply custom style
        if (customStyle.hasPen()) {
            canvas->setPen(customStyle.getPen());
        }
        if (customStyle.hasBrush()) {
            canvas->setBrush(customStyle.getBrush());
        }

        // Render shape
        styledShape.accept(*renderer);
    }


    void RenderingContext::renderWithTransform(const StyledShape& styledShape,
        float translateX, float translateY,
        float scale ) {
        canvas->save();  // Save current state
        canvas->translate(translateX, translateY);
        canvas->scale(scale, scale);

        render(styledShape);

        canvas->restore();  // Restore previous state
    }


    // CANVAS OPERATIONS


    void RenderingContext::drawText(float x, float y, const std::string& text,
        float fontSize ) {
        canvas->drawText(x, y, text, fontSize);
    }


    void RenderingContext::drawLine(float x1, float y1, float x2, float y2) {
        canvas->drawLine(x1, y1, x2, y2);
    }


    // SAVE/EXPORT



    bool RenderingContext::saveToFile(const std::string& filename) {
        return canvas->saveToFile(filename);
    }


    std::string RenderingContext::exportToString() const {
        return canvas->exportToSVG();
    }


    // GETTERS - For Advanced Operations



    std::shared_ptr<ICanvas> RenderingContext::getCanvas() { return canvas; }


    std::shared_ptr<IShapeVisitor> RenderingContext::getRenderer() { return renderer; }

    // UTILITY METHODS

    float RenderingContext::getWidth() const { return canvas->getWidth(); }
    float RenderingContext::getHeight() const { return canvas->getHeight(); }

    bool RenderingContext::isValid() const {
        return canvas != nullptr && renderer != nullptr;
    }
