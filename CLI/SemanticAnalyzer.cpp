#include "SemanticAnalyzer.h"
#include <string>
#include <iostream>

bool SemanticAnalyzer::isValidCommand(const SToken& cmd) {
    currentCmd = cmd;
    return validCmd.find(cmd) != validCmd.end();
}

bool SemanticAnalyzer::isValidArgument(const SToken& arg) {
    currentArg = arg;
    if (validCmd.find(currentCmd) == validCmd.end()) return false;
    auto& argsMap = validCmd.at(currentCmd);
    return argsMap.find(arg) != argsMap.end();
}

bool SemanticAnalyzer::isValidValue(const SToken& val) {
    if (validCmd.find(currentCmd) == validCmd.end()) return false;
    auto& argsMap = validCmd.at(currentCmd);
    if (argsMap.find(currentArg) == argsMap.end()) return false;
    auto& validator = argsMap.at(currentArg);
    return validator(val);
}


bool SemanticAnalyzer::isValidPositiveInteger(const SToken& token) {
    if (token.type != EType::Num) return false;
    try {
        int n = std::stoi(token.value);
        return n > 0;
    }
    catch (...) {
        return false;
    }
}

bool SemanticAnalyzer::isValidNumber(const SToken& token) {
    if (token.type != EType::Num) return false;
    try {
        std::stof(token.value); // Try to parse as float
        return true;
    }
    catch (...) {
        return false;
    }
}

bool SemanticAnalyzer::isValidString(const SToken& token) {
    return token.type == EType::Word;
}

bool SemanticAnalyzer::isValidAny(const SToken& token) {
    // Accept any token type (for strings, numbers, etc.)
    return true;
}

//bool SemanticAnalyzer:: isValidPositiveInteger(const SToken & token) {
//    for (char c : token.value) {
//        if (!isdigit(c)) return false;
//    }
//    return !token.value.empty();
//}
    