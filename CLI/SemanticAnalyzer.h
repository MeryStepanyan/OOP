#pragma once
#include "Parser.h"
#include <functional>
#include <unordered_map>
#include "Tokenizer.h"

class SemanticAnalyzer {
public:
    bool isValidCommand(const SToken& cmd);
    bool isValidArgument(const SToken& arg);
    bool isValidValue(const SToken& val);
    static  bool isValidPositiveInteger(const SToken&);
private:
    SToken currentCmd;
    SToken currentArg;

    std::unordered_map<SToken, std::unordered_map<SToken, std::function<bool(const SToken&)>>> validCmd = {
       { SToken("addslide", EType::Word), {{ SToken("pos", EType::Word), SemanticAnalyzer::isValidPositiveInteger }} },
       { SToken("removeslide", EType::Word), {{ SToken("pos", EType::Word), SemanticAnalyzer::isValidPositiveInteger }} }
    };

};
