
#include "Parser.h"

SCommand Parser::parseCommand() {
	bool gotCommand = false;
	SCommand cmd;

	while (true) {
		SToken token = m_tokenizer.getToken();

		if (token.type == EType::End) {
			break;
		}

	if (!gotCommand && token.type == EType::Word) {
		cmd.name = token.value;
		gotCommand = true;;
	}

	else {
		cmd.args.push_back(token.value);
	}
}
	return cmd;

}
