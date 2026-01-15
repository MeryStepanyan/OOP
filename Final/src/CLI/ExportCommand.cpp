
#include "ExportCommand.h"
#include "Presentation.h"


ExportCommand::ExportCommand() = default;
ExportCommand::ExportCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    std::string ExportCommand::getName() const { return "Export"; }

    void ExportCommand::execute(Editor& editor) {
        auto& presentation = editor.getPresentation();

        if (presentation.getSlideCount() == 0) {
            throw std::runtime_error("Cannot export: Presentation is empty");
        }

        ExportConfig config = parseConfiguration();

        if (config.singleFile) {
            exportPresentationToSingleFile(presentation, config);
        }
        else if (config.exportAll) {
            exportEntirePresentation(presentation, config);
        }
        else {
            exportSingleSlide(presentation, config.slideIndex, config);
        }
    }


    ExportCommand::ExportConfig ExportCommand::parseConfiguration() {
        ExportConfig config;

        try {
            std::string mode = getArgumentValue("mode");
            if (mode == "single" || mode == "combined" || mode == "onefile") {
                config.singleFile = true;
                config.exportAll = false;
            }
        }
        catch (...) {}

        try {
            config.slideIndex = parseSizeTArgument("slide");
            config.exportAll = false;
            config.singleFile = false;
        }
        catch (...) {
            if (!config.singleFile) {
                config.exportAll = true;
            }
        }

        try {
            config.outputPath = getArgumentValue("output");
        }
        catch (...) {
            if (config.singleFile) {
                config.outputPath = "presentation";
            }
            else if (!config.exportAll) {
                config.outputPath = "slide_" + std::to_string(config.slideIndex);
            }
            else {
                config.outputPath = "presentation_export";
            }
        }

        try {
            config.width = std::stof(getArgumentValue("width"));
        }
        catch (...) {}

        try {
            config.height = std::stof(getArgumentValue("height"));
        }
        catch (...) {}

        try {
            config.slideSpacing = std::stof(getArgumentValue("spacing"));
        }
        catch (...) {}

        try {
            config.backgroundColor = getArgumentValue("background");
        }
        catch (...) {}

        return config;
    }

    // ========================================================================
    // FIXED: Single File Export with Proper Transform Isolation
    // ========================================================================
    void ExportCommand::exportPresentationToSingleFile(Presentation& presentation, const ExportConfig& config) {
        std::cout << "\n========================================" << std::endl;
        std::cout << "   EXPORTING TO SINGLE FILE             " << std::endl;
        std::cout << "========================================\n" << std::endl;

        std::cout << "Total slides: " << presentation.getSlideCount() << std::endl;
        std::cout << "Output file: " << config.outputPath << ".svg" << std::endl;
        std::cout << "Slide size: " << config.width << "x" << config.height << std::endl;
        std::cout << "Spacing: " << config.slideSpacing << "px" << std::endl << std::endl;

        // Calculate total height (no spacing after last slide)
        float totalHeight = presentation.getSlideCount() * config.height +
            (presentation.getSlideCount() > 0 ? (presentation.getSlideCount() - 1) * config.slideSpacing : 0);

        // Create canvas
        auto canvas = std::make_shared<SVGCanvas>(config.width, totalHeight);
        canvas->clear();

        // CRITICAL FIX: Render each slide in isolated transform context
        for (size_t i = 0; i < presentation.getSlideCount(); ++i) {
            float yOffset = i * (config.height + config.slideSpacing);

            std::cout << "Adding slide " << (i + 1) << "/" << presentation.getSlideCount()
                << " at offset " << yOffset << "px... ";

            // Create isolated rendering context for this slide
            renderSlideIsolated(presentation, i, canvas, config, 0, yOffset);

            std::cout << "[OK]" << std::endl;
        }

        // Save to file
        std::string filename = config.outputPath + ".svg";
        if (!canvas->saveToFile(filename)) {
            throw std::runtime_error("Failed to save file: " + filename);
        }

        std::cout << "\n[SUCCESS] PRESENTATION EXPORTED!" << std::endl;
        std::cout << "=======================================" << std::endl;
        std::cout << "File: " << filename << std::endl;
        std::cout << "Dimensions: " << config.width << "x" << totalHeight << std::endl;
        std::cout << "Contains: " << presentation.getSlideCount() << " slides" << std::endl;
        std::cout << "=======================================" << std::endl << std::endl;
    }

 
    void ExportCommand::renderSlideIsolated(Presentation& presentation, size_t slideIndex,
        std::shared_ptr<SVGCanvas> canvas, const ExportConfig& config,
        float xOffset, float yOffset) {

        auto slide = presentation.getSlide(slideIndex);

        // Draw slide border (using direct canvas calls - no transform needed)
        auto borderPen = std::make_shared<Pen>("#CCCCCC", 2.0f);
        canvas->setPen(borderPen);
        canvas->drawLine(0, yOffset, config.width, yOffset);

        // Draw slide header (using direct canvas calls)
        auto headerPen = std::make_shared<Pen>("#333333", 1.0f);
        canvas->setPen(headerPen);

        //std::string slideLabel = "Slide " + std::to_string(slideIndex + 1);
        //canvas->drawText(10 + xOffset, 30 + yOffset, slideLabel, 20);

        //std::string shapeInfo = std::to_string(slide->getShapeCount()) + " shapes";
        //canvas->drawText(config.width - 100 + xOffset, 30 + yOffset, shapeInfo, 14);

        //// Draw separator line
        //canvas->drawLine(10 + xOffset, 40 + yOffset,
        //    config.width - 10 + xOffset, 40 + yOffset);

        // CRITICAL FIX: Save transform state BEFORE any modifications
        canvas->save();

        // Apply offset for this slide's content area
        canvas->translate(xOffset, yOffset + 50);

        // Create renderer for this isolated context
        auto renderer = std::make_shared<SVGRenderer>(canvas);

        // Render all shapes in this slide
        for (size_t i = 0; i < slide->getShapeCount(); ++i) {
            StyledShape styledShape = slide->getShape(i);

            // Apply style
            if (styledShape.getStyle().hasPen()) {
                canvas->setPen(styledShape.getStyle().getPen());
            }
            if (styledShape.getStyle().hasBrush()) {
                canvas->setBrush(styledShape.getStyle().getBrush());
            }

            // Render using visitor pattern
            styledShape.accept(*renderer);
        }

        // CRITICAL FIX: Restore transform state AFTER rendering
        canvas->restore();
    }


    void ExportCommand::exportEntirePresentation(Presentation& presentation, const ExportConfig& config) {
        std::cout << "\n========================================" << std::endl;
        std::cout << "   EXPORTING SEPARATE FILES             " << std::endl;
        std::cout << "========================================\n" << std::endl;

        std::cout << "Total slides: " << presentation.getSlideCount() << std::endl;
        std::cout << "Output pattern: " << config.outputPath << "_slide_*.svg" << std::endl;
        std::cout << "Canvas size: " << config.width << "x" << config.height << std::endl;
        std::cout << "Background: " << config.backgroundColor << std::endl << std::endl;

        for (size_t i = 0; i < presentation.getSlideCount(); ++i) {
            std::string filename = config.outputPath + "_slide_" + std::to_string(i + 1) + ".svg";

            std::cout << "Exporting slide " << (i + 1) << "/" << presentation.getSlideCount()
                << "... ";

            exportSlideToFile(presentation, i, filename, config);

            std::cout << "[OK] " << filename << std::endl;
        }

        std::cout << "\n[SUCCESS] EXPORT COMPLETE!" << std::endl;
        std::cout << "=======================================" << std::endl;
        std::cout << "Files created:" << std::endl;
        for (size_t i = 0; i < presentation.getSlideCount(); ++i) {
            std::cout << "  * " << config.outputPath << "_slide_" << (i + 1) << ".svg" << std::endl;
        }
        std::cout << "=======================================\n" << std::endl;
    }

    void ExportCommand::exportSingleSlide(Presentation& presentation, size_t slideIndex, const ExportConfig& config) {
        if (slideIndex >= presentation.getSlideCount()) {
            throw std::out_of_range(
                "Slide index " + std::to_string(slideIndex) + " is out of range. " +
                "Valid range: 0 to " + std::to_string(presentation.getSlideCount() - 1)
            );
        }

        std::string filename = config.outputPath + ".svg";

        std::cout << "\n========================================" << std::endl;
        std::cout << "   EXPORTING SINGLE SLIDE               " << std::endl;
        std::cout << "========================================\n" << std::endl;

        std::cout << "Slide: " << slideIndex << " of " << presentation.getSlideCount() << std::endl;
        std::cout << "Output: " << filename << std::endl;
        std::cout << "Canvas size: " << config.width << "x" << config.height << std::endl << std::endl;

        exportSlideToFile(presentation, slideIndex, filename, config);

        std::cout << "[SUCCESS] Slide exported successfully!" << std::endl;
        std::cout << "File: " << filename << "\n" << std::endl;
    }

    void ExportCommand::exportSlideToFile(Presentation& presentation, size_t slideIndex,
        const std::string& filename, const ExportConfig& config) {
        auto slide = presentation.getSlide(slideIndex);

        // Create fresh canvas for this slide
        auto canvas = std::make_shared<SVGCanvas>(config.width, config.height);
        auto renderer = std::make_shared<SVGRenderer>(canvas);
        RenderingContext context(canvas, renderer);

        // Clear with background
        context.setBackground(config.backgroundColor);

        // Add slide header
        auto headerPen = std::make_shared<Pen>("#333333", 1.0f);
        canvas->setPen(headerPen);

        //std::string slideLabel = "Slide " + std::to_string(slideIndex + 1);
        //context.drawText(10, 30, slideLabel, 20);

        //std::string shapeInfo = std::to_string(slide->getShapeCount()) + " shapes";
        //context.drawText(config.width - 100, 30, shapeInfo, 14);

        //// Draw separator
        //context.drawLine(10, 40, config.width - 10, 40);

        // Render all shapes
        for (size_t i = 0; i < slide->getShapeCount(); ++i) {
            StyledShape styledShape = slide->getShape(i);
            context.render(styledShape);
        }

        // Save to file
        if (!context.saveToFile(filename)) {
            throw std::runtime_error("Failed to save file: " + filename);
        }
    }
