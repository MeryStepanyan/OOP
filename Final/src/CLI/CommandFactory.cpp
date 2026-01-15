#include "CommandFactory.h"
#include "CommandRegistry.h"
#include "Commands.h"
#include "SaveCommand.h"
#include "LoadCommand.h"
#include "HelpCommand.h"
#include "ExportCommand.h"
#include "UndoRedoCommand.h"
#include "ExitCommand.h"
#include <stdexcept>

// ============================================================================
// REGISTRATION 
// ============================================================================

void CommandFactory::initializeRegistry() {
    auto& registry = getGlobalRegistry();

    // ========================================================================
    // ADDSLIDE
    // ========================================================================
    registry.registerCommand(
        MetaCommand("addslide",
            "Add a new slide to the presentation",
            [](const auto& args) { return std::make_unique<AddSlideCommand>(args); })
        .setLongDescription(
            "Creates a new empty slide and inserts it at the specified position. "
            "If no position is specified, adds the slide at the end.")
        .addArgument(ArgumentDescriptor("pos",
            "Position where to insert slide (0 = beginning)",
            false,
            "end of presentation",
            "number"))
        .addExample("addslide")
        .addExample("addslide -pos 0")
        .addExample("addslide -pos 2")
    );

    // ========================================================================
    // REMOVESLIDE
    // ========================================================================
    registry.registerCommand(
        MetaCommand("removeslide",
            "Remove a slide from the presentation",
            [](const auto& args) { return std::make_unique<RemoveSlideCommand>(args); })
        .setLongDescription(
            "Removes the slide at the specified position. "
            "This action can be undone with 'undo' command.")
        .addArgument(ArgumentDescriptor("pos",
            "Position of slide to remove",
            true,
            "",
            "number"))
        .addExample("removeslide -pos 0")
        .addExample("removeslide -pos 2")
    );

    // ========================================================================
    // ADDSHAPE
    // ========================================================================
    registry.registerCommand(
        MetaCommand("addshape",
            "Add a shape to a slide",
            [](const auto& args) { return std::make_unique<AddShapeCommand>(args); })
        .setLongDescription(
            "Creates and adds a shape to the specified slide. "
            "Supports custom colors and styles. "
            "Available shapes: circle, rectangle, square, line, triangle, ellipse, polygon, text, image.")
        .addArgument(ArgumentDescriptor("slide",
            "Target slide index",
            false,
            "last slide",
            "number"))
        .addArgument(ArgumentDescriptor("type",
            "Shape type (circle, rectangle, text, etc.)",
            true,
            "",
            "string"))
        .addArgument(ArgumentDescriptor("x",
            "X coordinate",
            true,
            "",
            "number"))
        .addArgument(ArgumentDescriptor("y",
            "Y coordinate",
            true,
            "",
            "number"))
        .addArgument(ArgumentDescriptor("fillcolor",
            "Fill color in hex format (#FF0000)",
            false,
            "default style",
            "color"))
        .addArgument(ArgumentDescriptor("strokecolor",
            "Stroke/outline color in hex format",
            false,
            "default style",
            "color"))
        .addArgument(ArgumentDescriptor("strokewidth",
            "Stroke width in pixels",
            false,
            "2.0",
            "number"))
        .addExample("addshape -type circle -x 100 -y 100 -radius 50")
        .addExample("addshape -type circle -x 100 -y 100 -radius 50 -fillcolor #FF0000")
        .addExample("addshape -type rectangle -x 50 -y 50 -width 100 -height 60")
        .addExample("addshape -type text -x 50 -y 50 -content \"Hello World\" -fontsize 24")
    );

    // ========================================================================
    // REMOVESHAPE
    // ========================================================================
    registry.registerCommand(
        MetaCommand("removeshape",
            "Remove a shape from a slide",
            [](const auto& args) { return std::make_unique<RemoveShapeCommand>(args); })
        .setLongDescription(
            "Removes the shape at the specified index from the specified slide. "
            "This action can be undone.")
        .addArgument(ArgumentDescriptor("slide",
            "Slide index",
            true,
            "",
            "number"))
        .addArgument(ArgumentDescriptor("shape",
            "Shape index to remove",
            true,
            "",
            "number"))
        .addExample("removeshape -slide 0 -shape 0")
        .addExample("removeshape -slide 1 -shape 2")
    );

    // ========================================================================
    // SAVE
    // ========================================================================
    registry.registerCommand(
        MetaCommand("save",
            "Save presentation to JSON file",
            [](const auto& args) { return std::make_unique<SaveCommand>(args); })
        .setLongDescription(
            "Saves the current presentation to a JSON file. "
            "Includes all slides, shapes, and styling information. "
            "The file can be loaded later with 'load' command.")
        .addArgument(ArgumentDescriptor("file",
            "Output filename",
            false,
            "presentation.json",
            "string"))
        .addExample("save")
        .addExample("save -file mywork.json")
    );

    // ========================================================================
    // LOAD
    // ========================================================================
    registry.registerCommand(
        MetaCommand("load",
            "Load presentation from JSON file",
            [](const auto& args) { return std::make_unique<LoadCommand>(args); })
        .setLongDescription(
            "Loads a presentation from a JSON file. "
            "WARNING: This replaces the current presentation!")
        .addArgument(ArgumentDescriptor("file",
            "Input filename",
            true,
            "",
            "string"))
        .addExample("load -file presentation.json")
        .addExample("load -file mywork.json")
    );

    // ========================================================================
    // EXPORT
    // ========================================================================
    registry.registerCommand(
        MetaCommand("export",
            "Export presentation to SVG format",
            [](const auto& args) { return std::make_unique<ExportCommand>(args); })
        .setLongDescription(
            "Exports the presentation or specific slides to SVG format. "
            "SVG files can be opened in web browsers or vector graphics editors.")
        .addArgument(ArgumentDescriptor("output",
            "Output filename",
            false,
            "presentation_export",
            "string"))
        .addArgument(ArgumentDescriptor("slide",
            "Export only this slide (optional)",
            false,
            "all slides",
            "number"))
        .addArgument(ArgumentDescriptor("width",
            "Canvas width",
            false,
            "800",
            "number"))
        .addArgument(ArgumentDescriptor("height",
            "Canvas height",
            false,
            "600",
            "number"))
        .addExample("export")
        .addExample("export -slide 0 -output slide0")
        .addExample("export -output myfile -width 1024 -height 768")
    );

    // ========================================================================
    // UNDO
    // ========================================================================
    registry.registerCommand(
        MetaCommand("undo",
            "Undo the last action",
            [](const auto& args) { return std::make_unique<UndoCommand>(args); })
        .setLongDescription(
            "Reverts the last action. "
            "Undoable actions include: adding/removing slides, adding/removing shapes.")
        .addExample("undo")
    );

    // ========================================================================
    // REDO
    // ========================================================================
    registry.registerCommand(
        MetaCommand("redo",
            "Redo the last undone action",
            [](const auto& args) { return std::make_unique<RedoCommand>(args); })
        .setLongDescription(
            "Re-applies the last undone action.")
        .addExample("redo")
    );

    // ========================================================================
    // EXIT
    // ========================================================================
    registry.registerCommand(
        MetaCommand("exit",
            "Exit the application",
            [](const auto& args) { return std::make_unique<ExitCommand>(args); })
        .setLongDescription(
            "Exits the application. "
            "Optionally saves the current file before exiting.")
        .addArgument(ArgumentDescriptor("autosave",
            "Automatically save before exit (yes/true)",
            false,
            "no",
            "string"))
        .addExample("exit")
        .addExample("exit -autosave yes")
        .addAlias("quit")
    );

    // ========================================================================
    // HELP
    // ========================================================================
    registry.registerCommand(
        MetaCommand("help",
            "Show help information",
            [](const auto& args) { return std::make_unique<HelpCommand>(args); })
        .setLongDescription(
            "Displays help information about available commands. "
            "Use 'help <command>' to get detailed information about a specific command.")
        .addArgument(ArgumentDescriptor("topic",
            "Command name to get help for",
            false,
            "all commands",
            "string"))
        .addExample("help")
        .addExample("help addslide")
        .addExample("help addshape")
    );
}

// ============================================================================
// FACTORY METHODS
// ============================================================================

std::unique_ptr<ICommand> CommandFactory::create(
    const std::string& cmdName,
    const std::unordered_map<std::string, std::string>& args)
{
    static bool initialized = false;
    if (!initialized) {
        initializeRegistry();
        initialized = true;
    }
    auto& registry = getGlobalRegistry();
    return registry.createCommand(cmdName, args);
}

bool CommandFactory::isRegistered(const std::string& cmdName) {
    auto& registry = getGlobalRegistry();
    return registry.hasCommand(cmdName);
}

std::vector<std::string> CommandFactory::getRegisteredCommands() {
    auto& registry = getGlobalRegistry();
    return registry.getAllCommandNames();
}