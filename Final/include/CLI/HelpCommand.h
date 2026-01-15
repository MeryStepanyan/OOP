#pragma once
#include "BaseCommand.h"
#include "Editor.h"
#include "CommandRegistry.h"
#include <iostream>

/**
 * HelpCommand - Автоматически генерирует справку из MetaCommand registry
 *
 * Больше НЕТ hardcoded текста!
 * Вся информация берется из MetaCommand описаний.
 */
class HelpCommand : public BaseCommand {
public:
    HelpCommand() = default;
    explicit HelpCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    std::string getName() const override { return "Help"; }

    void execute(Editor& editor) override {
        auto& registry = getGlobalRegistry();

        // Проверяем есть ли конкретная тема
        try {
            std::string topic = getArgumentValue("topic");

            // Показать детальную справку по команде
            registry.printCommandHelp(topic);
        }
        catch (...) {
            // Нет темы - показать общую справку
            showGeneralHelp(registry);
        }
    }

private:
    void showGeneralHelp(CommandRegistry& registry) {
        std::cout << "\n";
        std::cout << "===========================================\n";
        std::cout << "   PRESENTATION EDITOR\n";
        std::cout << "===========================================\n";
        std::cout << "\n";
        std::cout << "Type 'help <command>' for detailed information\n";
        std::cout << "Example: help addslide\n";

        // Registry автоматически выводит все команды
        registry.printAllCommandsShort();
    }
};