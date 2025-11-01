//CommandRegistry.cpp
// Separate file for command registration to reduce coupling
// This file can be excluded from builds if needed, or commands can register themselves

#include "CommandFactory.h"
#include "Commands.h"

// Static initialization - register all commands
namespace {
    struct CommandRegistryInitializer {
        CommandRegistryInitializer() {
            CommandFactory::registerCommand("addslide", 
                [](const std::unordered_map<std::string, std::string>& args) {
                    return std::make_unique<AddSlideCommand>(args);
                });
            
            CommandFactory::registerCommand("removeslide",
                [](const std::unordered_map<std::string, std::string>& args) {
                    return std::make_unique<RemoveSlideCommand>(args);
                });
            
            CommandFactory::registerCommand("save",
                [](const std::unordered_map<std::string, std::string>& args) {
                    return std::make_unique<SaveCommand>(args);
                });
            
            CommandFactory::registerCommand("load",
                [](const std::unordered_map<std::string, std::string>& args) {
                    return std::make_unique<LoadCommand>(args);
                });
            
            CommandFactory::registerCommand("addshape",
                [](const std::unordered_map<std::string, std::string>& args) {
                    return std::make_unique<AddShapeCommand>(args);
                });
            
            CommandFactory::registerCommand("removeshape",
                [](const std::unordered_map<std::string, std::string>& args) {
                    return std::make_unique<RemoveShapeCommand>(args);
                });
        }
    };
    
    // This will be initialized before main()
    static CommandRegistryInitializer initializer;
}

