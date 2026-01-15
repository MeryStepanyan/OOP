#pragma once
#include <functional>
#include <unordered_map>
#include "Tokenizer.h"

class SemanticAnalyzer {
public:
    SemanticAnalyzer() {}
    bool isValidCommand(const SToken& cmd);
    bool isValidArgument(const SToken& arg);
    bool isValidValue(const SToken& val);
    bool isPotentialArgument(const SToken& arg) const;
    static bool isValidPositiveInteger(const SToken&);
    static bool isValidNumber(const SToken&);
    static bool isValidString(const SToken&);
    static bool isValidAny(const SToken&);
private:
    SToken currentCmd;
    SToken currentArg;

    std::unordered_map<SToken, std::unordered_map<SToken, std::function<bool(const SToken&)>>> validCmd = {
       { SToken("addslide", EType::Word), {{ SToken("pos", EType::Word), SemanticAnalyzer::isValidPositiveInteger }} },
       { SToken("removeslide", EType::Word), {{ SToken("pos", EType::Word), SemanticAnalyzer::isValidPositiveInteger }} },
       { SToken("save", EType::Word), {
           { SToken("file", EType::Word), SemanticAnalyzer::isValidAny },
           { SToken("mode", EType::Word), SemanticAnalyzer::isValidAny }
       }},
       { SToken("load", EType::Word), {
           { SToken("file", EType::Word), SemanticAnalyzer::isValidAny },
           { SToken("mode", EType::Word), SemanticAnalyzer::isValidAny }
       }},
       { SToken("export", EType::Word), {
           { SToken("file", EType::Word), SemanticAnalyzer::isValidAny },
           { SToken("slide", EType::Word), SemanticAnalyzer::isValidNumber },
           { SToken("output", EType::Word), SemanticAnalyzer::isValidAny },
           { SToken("mode", EType::Word), SemanticAnalyzer::isValidAny },
           { SToken("width", EType::Word), SemanticAnalyzer::isValidNumber },
           { SToken("height", EType::Word), SemanticAnalyzer::isValidNumber },
           { SToken("spacing", EType::Word), SemanticAnalyzer::isValidNumber },
           { SToken("background", EType::Word), SemanticAnalyzer::isValidAny }
       }},
       { SToken("addshape", EType::Word), {
            // Common arguments
            { SToken("slide", EType::Word), SemanticAnalyzer::isValidNumber },
            { SToken("type", EType::Word), SemanticAnalyzer::isValidAny },
            { SToken("style", EType::Word), SemanticAnalyzer::isValidAny },

            // NEW: Color parameters
            { SToken("fillcolor", EType::Word), SemanticAnalyzer::isValidAny },
            { SToken("strokecolor", EType::Word), SemanticAnalyzer::isValidAny },
            { SToken("textcolor", EType::Word), SemanticAnalyzer::isValidAny },
            { SToken("strokewidth", EType::Word), SemanticAnalyzer::isValidNumber },
            { SToken("fillopacity", EType::Word), SemanticAnalyzer::isValidNumber },

            // Circle, Square, Ellipse, Triangle
            { SToken("x", EType::Word), SemanticAnalyzer::isValidNumber },
            { SToken("y", EType::Word), SemanticAnalyzer::isValidNumber },
            { SToken("radius", EType::Word), SemanticAnalyzer::isValidNumber },
            { SToken("size", EType::Word), SemanticAnalyzer::isValidNumber },

            // Rectangle, Image
            { SToken("width", EType::Word), SemanticAnalyzer::isValidNumber },
            { SToken("height", EType::Word), SemanticAnalyzer::isValidNumber },

            // Ellipse
            { SToken("radiusx", EType::Word), SemanticAnalyzer::isValidNumber },
            { SToken("radiusy", EType::Word), SemanticAnalyzer::isValidNumber },

            // Line
            { SToken("x1", EType::Word), SemanticAnalyzer::isValidNumber },
            { SToken("y1", EType::Word), SemanticAnalyzer::isValidNumber },
            { SToken("x2", EType::Word), SemanticAnalyzer::isValidNumber },
            { SToken("y2", EType::Word), SemanticAnalyzer::isValidNumber },

            // Triangle
            { SToken("x3", EType::Word), SemanticAnalyzer::isValidNumber },
            { SToken("y3", EType::Word), SemanticAnalyzer::isValidNumber },

            // Polygon
            { SToken("points", EType::Word), SemanticAnalyzer::isValidAny },

            // Text
            { SToken("content", EType::Word), SemanticAnalyzer::isValidAny },
            { SToken("fontsize", EType::Word), SemanticAnalyzer::isValidNumber },

            // Image
            { SToken("path", EType::Word), SemanticAnalyzer::isValidAny }
        }},
        { SToken("removeshape", EType::Word), {
            { SToken("slide", EType::Word), SemanticAnalyzer::isValidNumber },
            { SToken("shape", EType::Word), SemanticAnalyzer::isValidNumber }
        }},
        { SToken("undo", EType::Word), {} },
        { SToken("redo", EType::Word), {} },
        { SToken("help", EType::Word), {
            { SToken("topic", EType::Word), SemanticAnalyzer::isValidAny }
        }},
        // NEW: Exit commands
             { SToken("exit", EType::Word), {
                 { SToken("autosave", EType::Word), SemanticAnalyzer::isValidAny }  // Optional: exit -autosave yes
             }},
             { SToken("quit", EType::Word), {
                 { SToken("autosave", EType::Word), SemanticAnalyzer::isValidAny }  // Optional: quit -autosave yes
             }}
    };

};