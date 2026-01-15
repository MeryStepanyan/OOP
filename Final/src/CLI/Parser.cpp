#include "Parser.h"
#include "CommandFactory.h"
#include "SemanticAnalyzer.h"
#include <iostream>

enum class State {
    Start, Cmd, Arg, Value, End, Error
};

std::unique_ptr<ICommand> Parser::parseCommand(std::istream& input) {
    State currentState = State::Start;
    std::string commandName;
    std::unordered_map<std::string, std::string> args;
    std::string lastArg;

    std::unordered_map<State, std::unordered_map<EType, State>> fsm = {
        { State::Start, {{EType::Word, State::Cmd}} },
        { State::Cmd,   {{EType::Word, State::Arg}, {EType::EndLine, State::End}} },
        { State::Arg,   {{EType::Word, State::Value}, {EType::Num, State::Value}, {EType::Symbol, State::Value}} },  
        { State::Value, {{EType::Word, State::Arg}, {EType::Num, State::Value}, {EType::Symbol, State::Value}, {EType::EndLine, State::End}} },
    };

    while (currentState != State::End && currentState != State::Error) {
        SToken token = m_tokenizer->getToken(input);

        std::cout << "DEBUG: Token=[" << token.value << "] Type=" << static_cast<int>(token.type)
            << " State=" << static_cast<int>(currentState) << std::endl;

        if (token.type == EType::End) {
            break;
        }

        if (token.type == EType::Symbol && token.value == "-") {
            std::cout << "DEBUG: Skipping dash\n";
            continue;
        }

        auto it = fsm.find(currentState);
        if (it == fsm.end() || it->second.find(token.type) == it->second.end()) {
            std::cerr << "DEBUG: FSM failed at state " << static_cast<int>(currentState)
                << " for token type " << static_cast<int>(token.type) << std::endl;
            currentState = State::Error;
            break;
        }

        State prevState = currentState;
        State nextState = it->second.at(token.type);

        switch (nextState) {
        case State::Cmd:
            commandName = token.value;
            std::cout << "DEBUG: Command=" << commandName << std::endl;
            if (m_semanticAnalyzer && !m_semanticAnalyzer->isValidCommand(SToken(commandName, EType::Word))) {
                std::cerr << "Invalid command: " << commandName << std::endl;
                currentState = State::Error;
            }
            else {
                currentState = State::Cmd;
            }
            break;

        case State::Value:
            if (prevState == State::Arg || prevState == State::Cmd) {
                // Starting a new value
                args[lastArg] = token.value;
                std::cout << "DEBUG: Value for [" << lastArg << "]=" << token.value << std::endl;
                if (m_semanticAnalyzer && !m_semanticAnalyzer->isValidValue(token)) {
                    std::cerr << "Invalid value: " << token.value << std::endl;
                    currentState = State::Error;
                }
                else {
                    currentState = State::Value;
                }
            }
            else if (prevState == State::Value) {
                // *** BUILDING A VALUE - CONCATENATE ***
                // This handles:
                // - Filenames: "test.json" (word + symbol + word)
                // - Colors: "#FF0000" (symbol + word/num)
                // - Numbers in filenames: "file123.json"

                if (token.type == EType::Symbol) {
                    // Concatenate symbol (for ".", "#", etc.)
                    args[lastArg] += token.value;
                    std::cout << "DEBUG: Extended value (symbol) for [" << lastArg << "]=" << args[lastArg] << std::endl;
                    currentState = State::Value;
                }
                else if (token.type == EType::Num) {
                    // Check if this is part of a filename/color or a separate value
                    // If current value has non-numeric content, it's part of the value
                    bool hasNonNumeric = false;
                    for (char c : args[lastArg]) {
                        if (!std::isdigit(c) && c != '.' && c != '-') {
                            hasNonNumeric = true;
                            break;
                        }
                    }

                    if (hasNonNumeric) {
                        // Part of filename/color like "file123.json" or "#FF0000"
                        args[lastArg] += token.value;
                        std::cout << "DEBUG: Extended value (num) for [" << lastArg << "]=" << args[lastArg] << std::endl;
                        currentState = State::Value;
                    }
                    else {
                        // This shouldn't happen - a number after a number
                        std::cerr << "ERROR: Unexpected number after value\n";
                        currentState = State::Error;
                    }
                }
                else if (token.type == EType::Word) {
                    // Word after value - could be part of value or new argument
                    // Check with semantic analyzer
                    if (m_semanticAnalyzer && m_semanticAnalyzer->isPotentialArgument(token)) {
                        // It's a new argument
                        lastArg = token.value;
                        std::cout << "DEBUG: New Argument=" << lastArg << std::endl;
                        if (!m_semanticAnalyzer->isValidArgument(SToken(lastArg, EType::Word))) {
                            std::cerr << "Invalid argument: " << lastArg << std::endl;
                            currentState = State::Error;
                        }
                        else {
                            currentState = State::Arg;
                        }
                    }
                    else {
                        // Part of the value (like "json" in "test.json" or "FF0000" in "#FF0000")
                        args[lastArg] += token.value;
                        std::cout << "DEBUG: Extended value (word) for [" << lastArg << "]=" << args[lastArg] << std::endl;
                        currentState = State::Value;
                    }
                }
                else {
                    currentState = State::Error;
                }
            }
            break;

        case State::Arg:
            // When transitioning to Arg from Value state with a Word token
            if (prevState == State::Value && token.type == EType::Word) {
                if (m_semanticAnalyzer) {
                    // Check if this word is a valid argument for the current command
                    if (m_semanticAnalyzer->isPotentialArgument(token)) {
                        // It IS a valid argument, so start a new argument
                        lastArg = token.value;
                        std::cout << "DEBUG: New Argument=" << lastArg << std::endl;
                        if (!m_semanticAnalyzer->isValidArgument(SToken(lastArg, EType::Word))) {
                            std::cerr << "Invalid argument: " << lastArg << std::endl;
                            currentState = State::Error;
                        }
                        else {
                            currentState = State::Arg;
                        }
                    }
                    else {
                        // NOT a valid argument, so it must be part of the filename/value
                        args[lastArg] += token.value;
                        std::cout << "DEBUG: Extended value (word) for [" << lastArg << "]=" << args[lastArg] << std::endl;
                        currentState = State::Value;
                    }
                }
                else {
                    // No semantic analyzer - assume words after values are arguments
                    lastArg = token.value;
                    std::cout << "DEBUG: Argument=" << lastArg << std::endl;
                    currentState = State::Arg;
                }
            }
            else {
                // Normal argument handling (from Cmd state)
                lastArg = token.value;
                std::cout << "DEBUG: Argument=" << lastArg << std::endl;
                if (m_semanticAnalyzer && !m_semanticAnalyzer->isValidArgument(SToken(lastArg, EType::Word))) {
                    std::cerr << "Invalid argument: " << lastArg << std::endl;
                    currentState = State::Error;
                }
                else {
                    currentState = State::Arg;
                }
            }
            break;

        case State::End:
            break;

        default:
            break;
        }
    }

    if (currentState == State::Error) {
        std::cerr << "Syntax error in command\n";
        return nullptr;
    }

    std::cout << "DEBUG: Creating command [" << commandName << "] with " << args.size() << " args\n";
    return CommandFactory::create(commandName, args);
}