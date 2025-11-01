#pragma once
#include <functional>
#include <unordered_map>
#include "Tokenizer.h"

class SemanticAnalyzer {
public:
    SemanticAnalyzer(){}
    bool isValidCommand(const SToken& cmd);
    bool isValidArgument(const SToken& arg);
    bool isValidValue(const SToken& val);
    static bool isValidPositiveInteger(const SToken&);
    static bool isValidNumber(const SToken&);
    static bool isValidString(const SToken&);
    static bool isValidAny(const SToken&);
private:
    SToken currentCmd;
    SToken currentArg;

    std::unordered_map<SToken, std::unordered_map<SToken, std::function<bool(const SToken&)>>> validCmd = {
       { SToken("addslide", EType::Word), {{ SToken("pos", EType::Word), SemanticAnalyzer::isValidPositiveInteger }} },
       { SToken("removeslide", EType::Word), {{ SToken("at", EType::Word), SemanticAnalyzer::isValidPositiveInteger }} },
       { SToken("save", EType::Word), {{ SToken("file", EType::Word), SemanticAnalyzer::isValidAny }} },
       { SToken("load", EType::Word), {{ SToken("file", EType::Word), SemanticAnalyzer::isValidAny }} },
       { SToken("addshape", EType::Word), {
           { SToken("slide", EType::Word), SemanticAnalyzer::isValidNumber },
           { SToken("type", EType::Word), SemanticAnalyzer::isValidAny },
           { SToken("x", EType::Word), SemanticAnalyzer::isValidNumber },
           { SToken("y", EType::Word), SemanticAnalyzer::isValidNumber },
           { SToken("radius", EType::Word), SemanticAnalyzer::isValidNumber },
           { SToken("width", EType::Word), SemanticAnalyzer::isValidNumber },
           { SToken("height", EType::Word), SemanticAnalyzer::isValidNumber },
           { SToken("content", EType::Word), SemanticAnalyzer::isValidAny },
           { SToken("fontSize", EType::Word), SemanticAnalyzer::isValidNumber }
       }},
       { SToken("removeshape", EType::Word), {
           { SToken("slide", EType::Word), SemanticAnalyzer::isValidNumber },
           { SToken("index", EType::Word), SemanticAnalyzer::isValidNumber }
       }}
    };

};