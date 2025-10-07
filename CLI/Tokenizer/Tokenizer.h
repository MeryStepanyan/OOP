#pragma once
#include<string>
#include<istream>
#include "Token.h"

class Tokenizer {
public:
	Tokenizer(std::istream& input) :m_input(input) {}
	SToken getToken();
private:
	std::istream& m_input;
};
