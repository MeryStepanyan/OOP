#pragma once
#include "ICanvas.h"
#include "IShapeVisitor.h"
#include "StyledShape.h"
#include <memory>
#include <vector>

/**
 * RenderingContext - Facade for shape rendering
 *
 * SOLID Principles Applied:
 * - SRP: Single responsibility - coordinating rendering
 * - Facade Pattern: Simplifies complex rendering workflow
 * - DIP: Depends on abstractions (ICanvas, IShapeVisitor)
 *
 * This class provides a simple interface for rendering shapes.
 * It handles:
 * - Setting up the canvas
 * - Applying styles before rendering
 * - Coordinating between renderer and canvas
 * - Saving results
 *
 * Without this facade, clients would need to:
 * 1. Get style from StyledShape
 * 2. Extract pen and brush
 * 3. Apply to canvas
 * 4. Call visitor
 * 5. Save canvas
 *
 * With facade, it's just:
 *   context.render(styledShape);
 *
 * Usage:
 *   auto canvas = std::make_shared<SVGCanvas>(800, 600);
 *   auto renderer = std::make_shared<SVGRenderer>(canvas);
 *   RenderingContext context(canvas, renderer);
 *
 *   context.setBackground("#FFFFFF");
 *   context.render(styledShape1);
 *   context.render(styledShape2);
 *   context.saveToFile("output.svg");
 */
class RenderingContext {
private:
    std::shared_ptr<ICanvas> canvas;
    std::shared_ptr<IShapeVisitor> renderer;

public:
   
    RenderingContext(std::shared_ptr<ICanvas> c,
        std::shared_ptr<IShapeVisitor> r);

    void clear();
   
    void setBackground(const std::string& color);

    void setSize(float width, float height);


    void render(const StyledShape& styledShape);


    void render(const std::vector<StyledShape>& shapes);


    void renderShapes(const std::vector<std::shared_ptr<StyledShape>>& shapes);
  
   
    void renderWithStyle(const StyledShape& styledShape,
        const ShapeStyle& customStyle);

    void renderWithTransform(const StyledShape& styledShape,
        float translateX, float translateY,
        float scale = 1.0f);

    // CANVAS OPERATIONS
 

    void drawText(float x, float y, const std::string& text,
        float fontSize = 12.0f);

    void drawLine(float x1, float y1, float x2, float y2);

    bool saveToFile(const std::string& filename);


    std::string exportToString() const;


    std::shared_ptr<ICanvas> getCanvas();

 
    std::shared_ptr<IShapeVisitor> getRenderer();

 
    float getWidth() const;
    float getHeight() const;

    bool isValid() const;
};