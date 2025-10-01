#pragma once
#include <string>
#include "Token.h"
#include "Tokenizer.h"
#include "Commands.h"

class Parser {
public:
    Parser(Tokenizer& tokenizer);
    SCommand parseCommand();
private:
    Tokenizer& m_tokenizer;
};
