
#include "Stylepresets.h"


     ShapeStyle StylePresets::createDefaultCircle() {
        auto pen = std::make_shared<Pen>("#2E86DE", 2.0f);      // Blue outline
        auto brush = std::make_shared<SolidBrush>("#54A0FF", 0.8f);  // Light blue fill
        return ShapeStyle(pen, brush, "default-circle");
    }


     ShapeStyle StylePresets::createHighlightCircle() {
        auto pen = std::make_shared<Pen>("#C23616", 3.0f);      // Red outline
        auto brush = std::make_shared<SolidBrush>("#E84118", 0.9f);  // Light red fill
        return ShapeStyle(pen, brush, "highlight-circle");
    }


     ShapeStyle StylePresets::createSuccessCircle() {
        auto pen = std::make_shared<Pen>("#0FB952", 2.0f);      // Green outline
        auto brush = std::make_shared<SolidBrush>("#26DE81", 0.8f);  // Light green fill
        return ShapeStyle(pen, brush, "success-circle");
    }

    // ========================================================================
    // RECTANGLE STYLES
    // ========================================================================


     ShapeStyle StylePresets::createDefaultRectangle() {
        auto pen = std::make_shared<Pen>("#C23616", 2.0f);      // Red outline
        auto brush = std::make_shared<SolidBrush>("#E84118", 0.8f);  // Light red fill
        return ShapeStyle(pen, brush, "default-rectangle");
    }


     ShapeStyle StylePresets::createHighlightRectangle() {
        auto pen = std::make_shared<Pen>("#0FB952", 3.0f);      // Green outline
        auto brush = std::make_shared<SolidBrush>("#26DE81", 0.9f);  // Light green fill
        return ShapeStyle(pen, brush, "highlight-rectangle");
    }


     ShapeStyle StylePresets::createWarningRectangle() {
        auto pen = std::make_shared<Pen>("#F79F1F", 2.5f);      // Orange outline
        auto brush = std::make_shared<SolidBrush>("#FFC312", 0.85f); // Yellow fill
        return ShapeStyle(pen, brush, "warning-rectangle");
    }

    // ========================================================================
    // TEXT STYLES
    // ========================================================================

     ShapeStyle StylePresets::createDefaultText() {
        auto pen = std::make_shared<Pen>("#2C3E50", 1.0f);      // Dark gray
        return ShapeStyle(pen, nullptr, "default-text");
    }


     ShapeStyle StylePresets::createHeadingText() {
        auto pen = std::make_shared<Pen>("#000000", 1.5f);      // Black
        return ShapeStyle(pen, nullptr, "heading-text");
    }


     ShapeStyle StylePresets::createSubtitleText() {
        auto pen = std::make_shared<Pen>("#7F8C8D", 1.0f);      // Medium gray
        return ShapeStyle(pen, nullptr, "subtitle-text");
    }

     ShapeStyle StylePresets::createEmphasisText() {
        auto pen = std::make_shared<Pen>("#2E86DE", 1.2f);      // Blue
        return ShapeStyle(pen, nullptr, "emphasis-text");
    }

    // ========================================================================
    // IMAGE STYLES
    // ========================================================================


     ShapeStyle StylePresets::createDefaultImage() {
        auto pen = std::make_shared<Pen>("#0FB952", 2.0f);      // Green outline
        auto brush = std::make_shared<SolidBrush>("#26DE81", 0.3f);  // Very light green fill
        return ShapeStyle(pen, brush, "default-image");
    }


     ShapeStyle StylePresets::createFeaturedImage() {
        auto pen = std::make_shared<Pen>("#5758BB", 3.0f);      // Purple outline
        auto brush = std::make_shared<SolidBrush>("#6C5CE7", 0.2f);  // Light purple fill
        return ShapeStyle(pen, brush, "featured-image");
    }

    // ========================================================================
    // GENERIC STYLES
    // ========================================================================


     ShapeStyle StylePresets::createDefault() {
        auto pen = std::make_shared<Pen>("#333333", 2.0f);      // Dark gray outline
        auto brush = std::make_shared<SolidBrush>("#CCCCCC", 0.8f);  // Light gray fill
        return ShapeStyle(pen, brush, "default");
    }


     ShapeStyle StylePresets::createTransparent() {
        auto pen = std::make_shared<Pen>("#000000", 1.5f);      // Black outline
        return ShapeStyle(pen, nullptr, "transparent");
    }


     ShapeStyle StylePresets::createSolid(const std::string& color) {
        auto brush = std::make_shared<SolidBrush>(color, 1.0f);
        return ShapeStyle(nullptr, brush, "solid");
    }

    // ========================================================================
    // THEME STYLES
    // ========================================================================


     ShapeStyle StylePresets::createProfessionalShape() {
        auto pen = std::make_shared<Pen>("#34495E", 2.0f);      // Dark blue-gray
        auto brush = std::make_shared<SolidBrush>("#95A5A6", 0.7f);  // Light gray
        return ShapeStyle(pen, brush, "professional");
    }


     ShapeStyle StylePresets::createModernShape() {
        auto pen = std::make_shared<Pen>("#E74C3C", 2.5f);      // Red
        auto brush = std::make_shared<SolidBrush>("#3498DB", 0.85f); // Blue
        return ShapeStyle(pen, brush, "modern");
    }


     ShapeStyle StylePresets::createMinimalShape() {
        auto pen = std::make_shared<Pen>("#000000", 1.0f);      // Black
        auto brush = std::make_shared<SolidBrush>("#FFFFFF", 1.0f);  // White
        return ShapeStyle(pen, brush, "minimal");
    }
