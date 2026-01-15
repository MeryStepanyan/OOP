#pragma once
#include "MetaCommand.h"



// ============================================================================
// COMMAND REGISTRY - реестр всех команд
// ============================================================================

class CommandRegistry {
private:
    std::map<std::string, MetaCommand> commands;
    std::map<std::string, std::string> aliasMap;  // alias -> command name

public:
    
    void registerCommand(const MetaCommand& meta);
   
    const MetaCommand* getMetaCommand(const std::string& name)const;

   
    std::unique_ptr<ICommand> createCommand(
        const std::string& name,
        const std::unordered_map<std::string, std::string>& args) const;
   
    bool hasCommand(const std::string& name) const;
    
    std::vector<std::string> getAllCommandNames() const;

    
    void printAllCommandsShort() const;
    
    void printCommandHelp(const std::string& name) const;
private:
    void printCommandIfExists(const std::string& name) const;
};

// ============================================================================
// SINGLETON INSTANCE
// ============================================================================

 CommandRegistry& getGlobalRegistry();