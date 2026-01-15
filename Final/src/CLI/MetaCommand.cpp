#include "MetaCommand.h"
// ============================================================================
// METACOMMAND - метаданные о команде
// ============================================================================



        
MetaCommand::MetaCommand(const std::string& cmdName,
    const std::string& shortDesc,
    CreatorFunc creatorFunc)
    : name(cmdName),
    shortDescription(shortDesc),
    creator(creatorFunc) {
}

MetaCommand::MetaCommand() {
}

MetaCommand& MetaCommand::setLongDescription(const std::string& desc) {
    longDescription = desc;
    return *this;
}

MetaCommand& MetaCommand::addArgument(const ArgumentDescriptor& arg) {
    arguments.push_back(arg);
    return *this;
}

MetaCommand& MetaCommand::addExample(const std::string& example) {
    examples.push_back(example);
    return *this;
}

MetaCommand& MetaCommand::addAlias(const std::string& alias) {
    aliases.push_back(alias);
    return *this;
}


std::string MetaCommand::getName() const { return name; }
std::string MetaCommand::getShortDescription() const { return shortDescription; }
std::string MetaCommand::getLongDescription() const { return longDescription; }
const std::vector<ArgumentDescriptor>& MetaCommand::getArguments() const { return arguments; }
const std::vector<std::string>& MetaCommand::getExamples() const { return examples; }
const std::vector<std::string>& MetaCommand::getAliases() const { return aliases; }


std::unique_ptr<ICommand> MetaCommand::create(const std::unordered_map<std::string, std::string>& args) const {
    return creator(args);
}


void MetaCommand::printShortHelp() const {
    std::cout << "  " << name;


    for (const auto& arg : arguments) {
        if (arg.required) {
            std::cout << " -" << arg.name << " <" << arg.type << ">";
        }
    }


    std::cout << "\n      " << shortDescription << "\n";
}


void MetaCommand::printDetailedHelp() const {
    std::cout << "\n========================================\n";
    std::cout << "COMMAND: " << name << "\n";
    std::cout << "========================================\n\n";


    std::cout << shortDescription << "\n\n";


    if (!longDescription.empty()) {
        std::cout << "DESCRIPTION:\n";
        std::cout << "  " << longDescription << "\n\n";
    }


    if (!aliases.empty()) {
        std::cout << "ALIASES:\n";
        std::cout << "  ";
        for (size_t i = 0; i < aliases.size(); ++i) {
            std::cout << aliases[i];
            if (i < aliases.size() - 1) std::cout << ", ";
        }
        std::cout << "\n\n";
    }


    if (!arguments.empty()) {
        std::cout << "ARGUMENTS:\n";
        for (const auto& arg : arguments) {
            std::cout << "  -" << arg.name << " <" << arg.type << ">";

            if (arg.required) {
                std::cout << " (required)";
            }
            else {
                std::cout << " (optional)";
                if (!arg.defaultValue.empty()) {
                    std::cout << " [default: " << arg.defaultValue << "]";
                }
            }
            std::cout << "\n";
            std::cout << "      " << arg.description << "\n";
        }
        std::cout << "\n";
    }


    if (!examples.empty()) {
        std::cout << "EXAMPLES:\n";
        for (const auto& example : examples) {
            std::cout << "  > " << example << "\n";
        }
        std::cout << "\n";
    }
}
