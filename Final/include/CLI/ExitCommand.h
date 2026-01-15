#pragma once
#include "BaseCommand.h"
#include "Editor.h"
#include <iostream>
#include <cstdlib>  // Для std::exit()

/**
 * ExitCommand - Выход из приложения
 *
 * Простая реализация: использует std::exit(0) напрямую
 * Без исключений, без сложностей
 */
class ExitCommand : public BaseCommand {
public:
    ExitCommand() = default;
    explicit ExitCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    std::string getName() const override { return "Exit"; }

    void execute(Editor& editor) override {
        std::cout << "\n=========================================" << std::endl;

        // Проверка на несохраненные изменения
        if (editor.hasCurrentFile()) {
            std::cout << "Current file: " << editor.getCurrentFile() << std::endl;

            // Опциональное автосохранение
            try {
                std::string autosave = getArgumentValue("autosave");
                if (autosave == "yes" || autosave == "true") {
                    std::cout << "Auto-saving before exit..." << std::endl;
                    if (editor.saveToCurrentFile()) {
                        std::cout << "File saved successfully!" << std::endl;
                    }
                }
            }
            catch (...) {
                // Нет аргумента autosave - ничего страшного
            }
        }

        std::cout << "\nGoodbye!" << std::endl;
        std::cout << "=========================================\n" << std::endl;

        // Выход из программы
        std::exit(0);  // Немедленный выход с кодом успеха
    }
};