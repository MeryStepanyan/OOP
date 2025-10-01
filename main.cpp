////// CLI.cpp : This file contains the 'main' function. Program execution begins and ends there.

 


#include <iostream>
#include "Tokenizer.h"
#include "Parser.h"
#include "Commands.h"

int main() {
    Tokenizer tokenizer(std::cin);
    Parser parser(tokenizer);
     
    while (true) {
        SCommand cmd = parser.parseCommand();

        if (cmd.name.empty())
            break;
        std::cout << "Command name " << cmd.name << "\n";
        std::cout << "Args ";
        for (const auto& arg : cmd.args)
            std::cout << " [" << arg << "] ";
        std::cout<<"\n\n";
    }

}


