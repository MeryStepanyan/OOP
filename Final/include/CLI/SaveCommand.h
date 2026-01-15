#pragma once
#include "BaseCommand.h"
#include "Editor.h"
#include "Serializer.h"
#include <iostream>

/**
 * SaveCommand - SIMPLIFIED VERSION
 *
 * RESPONSIBILITIES:
 * ✅ Parse user arguments
 * ✅ Validate input
 * ✅ Call Serializer (does actual work)
 * ✅ Update editor state
 * ✅ Provide user feedback
 *
 * DOES NOT:
 * ❌ Generate JSON (Serializer does this)
 * ❌ Handle file I/O details (Serializer does this)
 * ❌ Serialize shapes/styles (Serializer does this)
 *
 * This follows the Single Responsibility Principle:
 * - SaveCommand = User Interface Layer
 * - Serializer = Data Transformation Layer
 */
class SaveCommand : public BaseCommand {
public:
    SaveCommand() = default;
    explicit SaveCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    std::string getName() const { return "save"; }

    void execute(Editor& editor) {
        // ====================================================================
        // COMMAND LAYER: Handle user interaction
        // ====================================================================

        // 1. Parse arguments (with default)
        std::string filename;
        try {
            filename = getArgumentValue("file");
        }
        catch (...) {
            filename = "presentation.json";
        }

        // 2. Validate input
        if (filename.empty()) {
            throw std::runtime_error("Filename cannot be empty");
        }

        // 3. Get presentation
        auto& presentation = editor.getPresentation();

        if (presentation.getSlideCount() == 0) {
            std::cout << "⚠️  Warning: Saving empty presentation" << std::endl;
        }

        // ====================================================================
        // SERIALIZER LAYER: Do actual serialization
        // ====================================================================

        try {
            // THIS IS THE KEY LINE - Serializer does all the work!
            bool success = Serializer::saveToFile(presentation, filename);

            if (success) {
                // ============================================================
                // COMMAND LAYER: Update state and provide feedback
                // ============================================================

                editor.setCurrentFile(filename);

                std::cout << "✅ Presentation saved successfully!" << std::endl;
                std::cout << "   File: " << filename << std::endl;
                std::cout << "   Slides: " << presentation.getSlideCount() << std::endl;

                // Show shape counts
                size_t totalShapes = 0;
                for (size_t i = 0; i < presentation.getSlideCount(); ++i) {
                    totalShapes += presentation.getSlide(i)->getShapeCount();
                }
                std::cout << "   Total shapes: " << totalShapes << std::endl;

            }
            else {
                throw std::runtime_error("Serializer returned false");
            }
        }
        catch (const std::exception& e) {
            // User-friendly error message
            throw std::runtime_error(
                "Failed to save presentation to '" + filename + "': " +
                std::string(e.what())
            );
        }
    }
};