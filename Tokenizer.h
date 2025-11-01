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
	bool operator==(const SToken& other) const {
		return value == other.value && type == other.type;
	}
};

namespace std {
	template <>
	struct hash<SToken> {
		std::size_t operator()(const SToken& t) const {
			return std::hash<std::string>()(t.value) ^ std::hash<int>()(static_cast<int>(t.type));
		}
	};
}

class Tokenizer {
public:

	SToken getToken(std::istream& input);

};