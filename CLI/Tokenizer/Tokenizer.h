#pragma once
#include<iostream>
#include <string>
#include <memory>
enum class EType {
	Value,
	Word,
	Num,
	String,
	Symbol,
	End,
	EndLine
};

struct SToken {
	std::string value;
	EType type;
	SToken() :value(""), type(EType::End) {}
	SToken(const std::string& value, EType type) :value(value), type(type) {}
};

class Tokenizer {
public:

	SToken getToken(std::istream& input);

};
