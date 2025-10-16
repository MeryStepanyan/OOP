#pragma once
#include <memory>
#include "ICommand.h"
#include "SemanticAnalyzer.h"
#include <istream>

class Tokenizer;

class Parser {
private:
    std::unique_ptr<Tokenizer> m_tokenizer;
    std::unique_ptr<SemanticAnalyzer> m_semanticAnalyzer;

public:
    Parser()
        : m_tokenizer(std::make_unique<Tokenizer>()),
        m_semanticAnalyzer(std::make_unique<SemanticAnalyzer>()){}

    std::unique_ptr<ICommand> parseCommand(std::istream& input);

};
