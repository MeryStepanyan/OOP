#include "Tokenizer.h"
#include <cctype>

SToken Tokenizer::getToken() {
	std::string buffer;

	while (m_input && std::isspace(m_input.peek())) {
		if (m_input.peek() == '\n'){
			m_input.get();
		return SToken("", EType::End);
	}
	m_input.get();
}
	if (!m_input)
		return SToken("", EType::End);

	char ch = m_input.peek();

	if (ch == '"' || ch == '\'') {
		char quote = m_input.get();
		while (m_input.get(ch)) {
			if (ch == quote)
				break;
			buffer += ch;
		}
		return SToken(buffer, EType::String);
	}
	
	if (std::isdigit(ch)) {
		while (m_input && (std::isdigit(m_input.peek()) || m_input.peek() == '.')) {
			m_input.get(ch);
			buffer += ch;
		}	
		return SToken(buffer, EType::Num);
	}

	if (std::isalpha(ch)) {
		while (m_input && std::isalnum(m_input.peek())) {
			m_input.get(ch);
			buffer += ch;
		}
		return SToken(buffer, EType::Word);
	}

	m_input.get(ch);
	buffer += ch;
	return SToken(buffer, EType::Symbol);
}
