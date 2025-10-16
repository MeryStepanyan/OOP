#pragma once
#include "Tokenizer.h"
#include "SemanticAnalyzer.h"
#include "commandFactory.h"
#include "ICommand.h"
#include <unordered_map>
#include <memory>
#include <variant>
#include <vector>


enum class State {
    Start,
    Cmd,
    Arg,
    Value,
    End,
    Error
};

std::unique_ptr<ICommand> Parser::parseCommand(std::istream& input) {
    State currentState = State::Start;
    std::string commandName; 
    std::unordered_map<std::string, std::string> args;
    std::string lastArg;

    std::unordered_map<State, std::unordered_map<EType, State>> fsm = {
        { State::Start, {{EType::Word, State::Cmd}} },
        { State::Cmd,   {{EType::Word, State::Arg}, {EType::EndLine, State::End}} },
        { State::Arg,   {{EType::Word, State::Value}} },
        { State::Value, {{EType::Word, State::Arg}, {EType::EndLine, State::End}} },
    };

    while (true) {
        SToken token = m_tokenizer->getToken(input);
        if (token.type == EType::End)
            break;

        
        auto it = fsm.find(currentState);
        if (it == fsm.end() || it->second.find(token.type) == it->second.end()) {
            currentState = State::Error;
            break;
        }

        currentState = it->second.at(token.type);

        switch (currentState) {
        case State::Cmd:
            commandName = token.value;  
            if (!m_semanticAnalyzer->isValidCommand(token)) {
                std::cerr << "Invalid command: " << token.value << std::endl;
                currentState = State::Error;
            }
            break;

        case State::Arg:
            lastArg = token.value;
            if (!m_semanticAnalyzer->isValidArgument(token)) {
                std::cerr << "Invalid argument: " << token.value << std::endl;
                currentState = State::Error;
            }
            break;

        case State::Value:
            args[lastArg] = token.value; 
            if (!m_semanticAnalyzer->isValidValue(token)) {
                std::cerr << "Invalid value: " << token.value << std::endl;
                currentState = State::Error;
            }
            break;

        case State::End:
            goto END_PARSE;

        default:
            break;
        }

        if (currentState == State::Error)
            break;
    }

END_PARSE:
    if (currentState == State::Error) {
        std::cerr << "Syntax error in command\n";
        return nullptr;
    }

    return CommandFactory::create(commandName, args);
}

