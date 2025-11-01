#pragma once
#include <memory>
#include "ICommand.h"
#include "Tokenizer.h"
#include "SemanticAnalyzer.h"
#include <istream>

class Parser {
private:
    std::unique_ptr<Tokenizer> m_tokenizer;
    SemanticAnalyzer* m_semanticAnalyzer;  // RAW POINTER - ոչ ownership

public:
    // Constructor - inject անում ենք analyzer-ը
    Parser(SemanticAnalyzer* analyzer)
        : m_tokenizer(std::make_unique<Tokenizer>()),
        m_semanticAnalyzer(analyzer) {
    }

    std::unique_ptr<ICommand> parseCommand(std::istream& input);
};