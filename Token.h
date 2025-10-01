#pragma once
#include<iostream>

enum class EType {
	Value,
	Word,
	Num,
	String,
	Symbol,
	End
};

struct SToken {
	std::string value;
	EType type;
	SToken(const std::string& value, EType type) :value(value), type(type) {}
};


