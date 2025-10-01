
#include "Parser.h"

Parser::Parser(Tokenizer& tokenizer):m_tokenizer(tokenizer){}

SCommand Parser::parseCommand() {
	SCommand cmd;
	SToken token = m_tokenizer.getToken();

	if (token.type == EType::Word || token.type==EType::String) {
		cmd.name = token.value;
	}
	else {
		return cmd;
	}

	while (true) {
		token = m_tokenizer.getToken();
		if (token.type != EType::End) {
			cmd.args.push_back(token.value);
		}
		break;
	}
	return cmd;

}
