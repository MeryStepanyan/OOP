#pragma once
#include "BaseCommand.h"
#include "RemoveShapeAction.h"
#include "Editor.h"
#include <stdexcept>

class RemoveShapeCommand : public BaseCommand {
public:
    RemoveShapeCommand() = default;
    explicit RemoveShapeCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    std::string getName() const { return "RemoveShape"; }

    void execute(Editor& editor) {
        // Validate required arguments
        validateRequiredArgument("slide");
        validateRequiredArgument("shape");

        size_t slideIndex = parseSizeTArgument("slide");
        size_t shapeIndex = parseSizeTArgument("shape");

        // Validate indices
        auto& presentation = editor.getPresentation();
        if (slideIndex >= presentation.getSlideCount()) {
            throw std::out_of_range(
                "Slide index " + std::to_string(slideIndex) + " is out of range. " +
                "Valid range: 0 to " + std::to_string(presentation.getSlideCount() - 1)
            );
        }

        auto slide = presentation.getSlide(slideIndex);
        if (shapeIndex >= slide->getShapeCount()) {
            throw std::out_of_range(
                "Shape index " + std::to_string(shapeIndex) + " is out of range. " +
                "Valid range: 0 to " + std::to_string(slide->getShapeCount() - 1)
            );
        }

        // Create and execute action
        auto action = std::make_unique<RemoveShapeAction>(slideIndex, shapeIndex);
        editor.doAction(std::move(action));
    }
};