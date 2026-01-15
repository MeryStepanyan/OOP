#pragma once
#include "ShapeStyle.h"
#include "Pen.h"
#include "SolidBrush.h"

 
class StylePresets {
public:
    // ========================================================================
    // CIRCLE STYLES
    // ========================================================================


    static ShapeStyle createDefaultCircle();

  
    static ShapeStyle createHighlightCircle();

 
    static ShapeStyle createSuccessCircle();
    // ========================================================================
    // RECTANGLE STYLES
    // ========================================================================

  
    static ShapeStyle createDefaultRectangle();
    
    static ShapeStyle createHighlightRectangle();

  
    static ShapeStyle createWarningRectangle();
    // ========================================================================
    // TEXT STYLES
    // ========================================================================

    static ShapeStyle createDefaultText();

    static ShapeStyle createHeadingText();
 
    static ShapeStyle createSubtitleText();

    static ShapeStyle createEmphasisText();
    // ========================================================================
    // IMAGE STYLES
    // ========================================================================

    
    static ShapeStyle createDefaultImage();

  
    static ShapeStyle createFeaturedImage();

    // ========================================================================
    // GENERIC STYLES
    // ========================================================================

   
    static ShapeStyle createDefault();

    
    static ShapeStyle createTransparent();

   
    static ShapeStyle createSolid(const std::string& color);

    // ========================================================================
    // THEME STYLES
    // ========================================================================

   
    static ShapeStyle createProfessionalShape();
  
    static ShapeStyle createModernShape();
  
    static ShapeStyle createMinimalShape();
};