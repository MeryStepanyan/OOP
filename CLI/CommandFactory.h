
#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <map>
#include "ICommand.h"

class CommandFactory {
public:
    using CreatorFunc = std::function<std::unique_ptr<ICommand>(
        const std::unordered_map<std::string, std::string>&)>;

   
    static void registerCommand(const std::string& name, CreatorFunc creator);
    
    
    static std::unique_ptr<ICommand> create(
        const std::string& cmdName,
        const std::unordered_map<std::string, std::string>& args);
    
    
    static std::vector<std::string> getRegisteredCommands();
    
  
    static bool isRegistered(const std::string& cmdName);

private:

    static std::map<std::string, CreatorFunc>& getRegistry();
    
    CommandFactory() = delete; 
    CommandFactory(const CommandFactory&) = delete;
    CommandFactory& operator=(const CommandFactory&) = delete;
};
