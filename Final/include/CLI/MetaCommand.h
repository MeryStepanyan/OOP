#pragma once
#include "ICommand.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <unordered_map>

// ============================================================================
// ARGUMENT DESCRIPTOR 
// ============================================================================

struct ArgumentDescriptor {
    std::string name;           // Имя аргумента (например, "pos")
    std::string description;    // Описание ("Position where to insert slide")
    bool required;              // Обязательный или нет
    std::string defaultValue;   // Значение по умолчанию (если есть)
    std::string type;           // Тип ("number", "string", "color", etc.)

    ArgumentDescriptor(const std::string& n,
        const std::string& desc,
        bool req = false,
        const std::string& defVal = "",
        const std::string& t = "string")
        : name(n), description(desc), required(req), defaultValue(defVal), type(t) {
    }
};

// ============================================================================
// METACOMMAND - метаданные о команде
// ============================================================================

class MetaCommand {
private:
    std::string name;                           // Имя команды
    std::string shortDescription;               // Краткое описание (одна строка)
    std::string longDescription;                // Подробное описание
    std::vector<ArgumentDescriptor> arguments;  // Список аргументов
    std::vector<std::string> examples;          // Примеры использования
    std::vector<std::string> aliases;           // Альтернативные имена (quit для exit)


    using CreatorFunc = std::function<std::unique_ptr<ICommand>(
        const std::unordered_map<std::string, std::string>&)>;
    CreatorFunc creator;

public:

    MetaCommand(const std::string& cmdName,
        const std::string& shortDesc,
        CreatorFunc creatorFunc);

    MetaCommand();

    MetaCommand& setLongDescription(const std::string& desc);

    MetaCommand& addArgument(const ArgumentDescriptor& arg);

    MetaCommand& addExample(const std::string& example);
    MetaCommand& addAlias(const std::string& alias);

    std::string getName() const;
    std::string getShortDescription() const;
    std::string getLongDescription() const;
    const std::vector<ArgumentDescriptor>& getArguments() const;
    const std::vector<std::string>& getExamples() const;
    const std::vector<std::string>& getAliases() const;


    std::unique_ptr<ICommand> create(const std::unordered_map<std::string, std::string>& args) const;


    void printShortHelp() const;


    void printDetailedHelp() const;
};

// ============================================================================
// COMMAND REGISTRY - реестр всех команд
// ============================================================================
