#include "Tokenizer.h"
#include <cctype>


SToken Tokenizer::getToken(std::istream& input) {
    char ch;

    while (input.get(ch)) {
        if (std::isspace(ch)) {
            if (ch == '\n') return SToken("", EType::EndLine);
            continue;
        }

        std::string buffer;

        if (std::isalpha(ch)) {
            buffer += ch;
            while (input.peek() != EOF && std::isalnum(input.peek())) {
                input.get(ch);
                buffer += ch;
            }
            return SToken(buffer, EType::Word);
        }

        if (std::isdigit(ch)) {
            buffer += ch;
            while (input.peek() != EOF && (std::isdigit(input.peek()) || input.peek() == '.')) {
                input.get(ch);
                buffer += ch;
            }
            return SToken(buffer, EType::Num);
        }

        if (ch == '"' || ch == '\'') {
            char quote = ch;
            while (input.get(ch) && ch != quote) {
                buffer += ch;
            }
            return SToken(buffer, EType::String);
        }

        buffer += ch;
        return SToken(buffer, EType::Symbol);
    }

    return SToken("", EType::End);
}
