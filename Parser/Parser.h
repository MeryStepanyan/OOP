#pragma once
#include <string>
#include "Token.h"
#include "Tokenizer.h"
#include "Commands.h"

class Parser {
public:
    Parser(std::istream& stream):m_tokenizer(stream){}
    SCommand parseCommand();
private:
    Tokenizer m_tokenizer;
};
