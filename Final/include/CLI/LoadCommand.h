#pragma once
#include "BaseCommand.h"
#include "Editor.h"
#include "Serializer.h"
#include <iostream>
#include <fstream>

/**
 * LoadCommand - SIMPLIFIED VERSION
 *
 * RESPONSIBILITIES:
 * ✅ Parse user arguments
 * ✅ Validate file exists
 * ✅ Call Serializer (does actual work)
 * ✅ Update editor state
 * ✅ Provide user feedback
 *
 * DOES NOT:
 * ❌ Parse JSON (Serializer does this)
 * ❌ Create shapes (Serializer does this)
 * ❌ Deserialize styles (Serializer does this)
 *
 * This follows the Single Responsibility Principle:
 * - LoadCommand = User Interface Layer
 * - Serializer = Data Transformation Layer
 */
class LoadCommand : public BaseCommand {
public:
    LoadCommand() = default;
    explicit LoadCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    std::string getName() const { return "Load"; }

    void execute(Editor& editor) {
        // ====================================================================
        // COMMAND LAYER: Handle user interaction
        // ====================================================================

        // 1. Parse arguments (required)
        std::string filename;
        try {
            filename = getArgumentValue("file");
        }
        catch (...) {
            throw std::runtime_error("Missing required argument: -file <filename>");
        }

        // 2. Validate file exists
        std::ifstream checkFile(filename);
        if (!checkFile.is_open()) {
            throw std::runtime_error(
                "File not found: '" + filename + "'\n" +
                "Please check the filename and try again."
            );
        }
        checkFile.close();

        // 3. Get presentation (will be cleared by Serializer)
        auto& presentation = editor.getPresentation();

        // Show warning if current presentation has unsaved changes
        if (presentation.getSlideCount() > 0 && editor.hasCurrentFile()) {
            std::cout << "⚠️  Warning: Loading will replace current presentation" << std::endl;
        }

        // ====================================================================
        // SERIALIZER LAYER: Do actual deserialization
        // ====================================================================

        try {
            // THIS IS THE KEY LINE - Serializer does all the work!
            bool success = Serializer::loadFromFile(presentation, filename);

            if (success) {
                // ============================================================
                // COMMAND LAYER: Update state and provide feedback
                // ============================================================

                editor.setCurrentFile(filename);

                std::cout << "✅ Presentation loaded successfully!" << std::endl;
                std::cout << "   File: " << filename << std::endl;
                std::cout << "   Total slides: " << presentation.getSlideCount() << std::endl;

                // Show detailed slide information
                size_t totalShapes = 0;
                for (size_t i = 0; i < presentation.getSlideCount(); ++i) {
                    auto slide = presentation.getSlide(i);
                    size_t shapeCount = slide->getShapeCount();
                    totalShapes += shapeCount;

                    std::cout << "   Slide " << i << ": "
                        << shapeCount << " shape"
                        << (shapeCount != 1 ? "s" : "") << std::endl;
                }

                std::cout << "   Total shapes: " << totalShapes << std::endl;

                // Verify styles were loaded
                if (totalShapes > 0) {
                    auto firstSlide = presentation.getSlide(0);
                    if (firstSlide->getShapeCount() > 0) {
                        StyledShape firstShape = firstSlide->getShape(0);
                        if (firstShape.getStyle().hasPen() || firstShape.getStyle().hasBrush()) {
                            std::cout << "   ✅ Styles loaded successfully" << std::endl;
                        }
                        else {
                            std::cout << "   ⚠️  Warning: No styles found (legacy format?)" << std::endl;
                        }
                    }
                }

            }
            else {
                throw std::runtime_error("Serializer returned false");
            }
        }
        catch (const std::exception& e) {
            // User-friendly error message
            throw std::runtime_error(
                "Failed to load presentation from '" + filename + "': " +
                std::string(e.what()) + "\n" +
                "Please check if the file is valid JSON format."
            );
        }
    }
};