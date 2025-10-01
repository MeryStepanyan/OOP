#include "Tokenizer.h"

SToken Tokenizer::getToken() {
	std::string buffer;
	EType type = EType::Value;

	while (m_input && std::isspace(m_input.peek()))
		m_input.get();

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
		type = EType::String;
		return SToken(buffer, type);
	}
	
	if (std::isdigit(ch)) {
		while (m_input && (std::isdigit(m_input.peek()) || m_input.peek() == '.')) {
			m_input.get(ch);
			buffer += ch;
		}	
		type = EType::Num;
		return SToken(buffer, type);
	}

	if (std::isalpha(ch)) {
		while (m_input && std::isalnum(m_input.peek())) {
			m_input.get(ch);
			buffer += ch;
		}
		type = EType::Word;
		return SToken(buffer, type);
	}

	m_input.get(ch);
	buffer += ch;
	type = EType::Symbol;
	return SToken(buffer, type);
}
