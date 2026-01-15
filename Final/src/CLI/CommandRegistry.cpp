
#include "CommandRegistry.h"

// ============================================================================
// COMMAND REGISTRY - реестр всех команд
// ============================================================================

    void CommandRegistry::registerCommand(const MetaCommand& meta) {
        commands[meta.getName()] = meta;

        // Регистрируем aliases
        for (const auto& alias : meta.getAliases()) {
            aliasMap[alias] = meta.getName();
        }
    }


    const MetaCommand* CommandRegistry::getMetaCommand(const std::string& name) const {

        auto it = commands.find(name);
        if (it != commands.end()) {
            return &it->second;
        }


        auto aliasIt = aliasMap.find(name);
        if (aliasIt != aliasMap.end()) {
            return getMetaCommand(aliasIt->second);
        }

        return nullptr;
    }


    std::unique_ptr<ICommand> CommandRegistry::createCommand(
        const std::string& name,
        const std::unordered_map<std::string, std::string>& args) const {

        const MetaCommand* meta = getMetaCommand(name);
        if (!meta) {
            throw std::invalid_argument("Unknown command: " + name);
        }

        return meta->create(args);
    }


    bool CommandRegistry::hasCommand(const std::string& name) const {
        return getMetaCommand(name) != nullptr;
    }


    std::vector<std::string>CommandRegistry:: getAllCommandNames() const {
        std::vector<std::string> names;
        for (const auto& [name, meta] : commands) {
            names.push_back(name);
        }
        return names;
    }


    void CommandRegistry::printAllCommandsShort() const {
        std::cout << "\n===========================================\n";
        std::cout << "   AVAILABLE COMMANDS\n";
        std::cout << "===========================================\n\n";

        std::cout << "SLIDE MANAGEMENT:\n";
        printCommandIfExists("addslide");
        printCommandIfExists("removeslide");

        std::cout << "\nSHAPE MANAGEMENT:\n";
        printCommandIfExists("addshape");
        printCommandIfExists("removeshape");

        std::cout << "\nFILE OPERATIONS:\n";
        printCommandIfExists("save");
        printCommandIfExists("load");
        printCommandIfExists("export");

        std::cout << "\nUNDO/REDO:\n";
        printCommandIfExists("undo");
        printCommandIfExists("redo");

        std::cout << "\nUTILITY:\n";
        printCommandIfExists("help");
        printCommandIfExists("exit");

        std::cout << "\nFor detailed help: help <command>\n";
        std::cout << "Example: help addslide\n\n";
    }


    void CommandRegistry::printCommandHelp(const std::string& name) const {
        const MetaCommand* meta = getMetaCommand(name);
        if (!meta) {
            std::cout << "Unknown command: " << name << "\n";
            std::cout << "Type 'help' to see available commands.\n";
            return;
        }

        meta->printDetailedHelp();
    }


    void CommandRegistry::printCommandIfExists(const std::string& name) const {
        const MetaCommand* meta = getMetaCommand(name);
        if (meta) {
            meta->printShortHelp();
        }
    }


// ============================================================================
// SINGLETON INSTANCE
// ============================================================================

 CommandRegistry& getGlobalRegistry() {
    static CommandRegistry registry;
    return registry;
}