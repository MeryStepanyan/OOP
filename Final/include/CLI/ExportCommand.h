#pragma once
#include "BaseCommand.h"
#include "Editor.h"
#include "SVGCanvas.h"
#include "SVGRenderer.h"
#include "RenderingContext.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>

class ExportCommand : public BaseCommand {
public:
    ExportCommand();
    ExportCommand(const std::unordered_map<std::string, std::string>& args);
    std::string getName() const;

    void execute(Editor& editor);
private:
    struct ExportConfig {
        bool exportAll = true;
        bool singleFile = false;
        size_t slideIndex = 0;
        std::string outputPath = "presentation_export";
        float width = 800.0f;
        float height = 600.0f;
        float slideSpacing = 50.0f;
        std::string backgroundColor = "#FFFFFF";
    };

    ExportConfig parseConfiguration();

    // ========================================================================
    // FIXED: Single File Export with Proper Transform Isolation
    // ========================================================================
    void exportPresentationToSingleFile(Presentation& presentation, const ExportConfig& config);

    // ========================================================================
    // CRITICAL FIX: Isolated Slide Rendering
    // ========================================================================
    void renderSlideIsolated(Presentation& presentation, size_t slideIndex,
        std::shared_ptr<SVGCanvas> canvas, const ExportConfig& config,
        float xOffset, float yOffset);

    // ========================================================================
    // Separate Files Export
    // ========================================================================
    void exportEntirePresentation(Presentation& presentation, const ExportConfig& config);
    

    void exportSingleSlide(Presentation& presentation, size_t slideIndex, const ExportConfig& config);
    void exportSlideToFile(Presentation& presentation, size_t slideIndex,
    const std::string& filename, const ExportConfig& config);
};