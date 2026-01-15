#pragma once
#include "BaseCommand.h"
#include "AddSlideAction.h"
#include "Editor.h"

class AddSlideCommand : public BaseCommand {
public:
    AddSlideCommand() = default;
    explicit AddSlideCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    std::string getName() const  { return "AddSlide"; }
    void execute(Editor& editor) {

        size_t position = parseSizeTArgument("pos",
            editor.getPresentation().getSlideCount());


        if (position > editor.getPresentation().getSlideCount()) {
            throw std::out_of_range(
                "[AddSlide] Position " + std::to_string(position) + " is out of range. " +
                "Valid range: 0 to " + std::to_string(editor.getPresentation().getSlideCount())
            );
        }

        auto slide = std::make_shared<Slide>();

        auto action = std::make_unique<AddSlideAction>(slide, position);
        std::cout << " Slide added at position " << position << std::endl;
        editor.doAction(std::move(action));

        std::cout << "Total slides: " << editor.getPresentation().getSlideCount()
            << std::endl;
    }
};