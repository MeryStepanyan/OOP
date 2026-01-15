#pragma once
#include "BaseCommand.h"
#include "RemoveSlideAction.h"
#include "Editor.h"
#include <stdexcept>

class RemoveSlideCommand : public BaseCommand {
public:
    RemoveSlideCommand() = default;
    explicit RemoveSlideCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    std::string getName() const { return "RemoveSlide"; }

    void execute(Editor& editor) {
        // Validate required argument
        validateRequiredArgument("pos");

        size_t position = parseSizeTArgument("pos");

        // Validate position
        auto& presentation = editor.getPresentation();
        if (position >= presentation.getSlideCount()) {
            throw std::out_of_range(
                "Position " + std::to_string(position) + " is out of range. " +
                "Valid range: 0 to " + std::to_string(presentation.getSlideCount() - 1)
            );
        }

        if (presentation.getSlideCount() == 0) {
            throw std::logic_error("Cannot remove slide from empty presentation");
        }

        // Create and execute action
        auto action = std::make_unique<RemoveSlideAction>(position);
        editor.doAction(std::move(action));

        std::cout << "Total slides: " << presentation.getSlideCount() << std::endl;
    }
};