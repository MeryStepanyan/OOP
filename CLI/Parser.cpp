
#include "Parser.h"
#include "CommandFactory.h"
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
        { State::Arg,   {{EType::Word, State::Value}, {EType::Num, State::Value}} },  
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
        currentState = it->second.at(token.type);

        switch (currentState) {
        case State::Cmd:
            commandName = token.value;
            std::cout << "DEBUG: Command=" << commandName << std::endl;
            if (m_semanticAnalyzer && !m_semanticAnalyzer->isValidCommand(SToken(commandName, EType::Word))) {
                std::cerr << "Invalid command: " << commandName << std::endl;
                currentState = State::Error;
            }
            break;

        case State::Arg:
            lastArg = token.value;
            std::cout << "DEBUG: Argument=" << lastArg << std::endl;
            if (m_semanticAnalyzer && !m_semanticAnalyzer->isValidArgument(SToken(lastArg, EType::Word))) {
                std::cerr << "Invalid argument: " << lastArg << std::endl;
                currentState = State::Error;
            }
            break;

        case State::Value:
           
            if (prevState == State::Arg || prevState == State::Cmd) {
                args[lastArg] = token.value;
                std::cout << "DEBUG: Value for [" << lastArg << "]=" << token.value << std::endl;
                if (m_semanticAnalyzer && !m_semanticAnalyzer->isValidValue(token)) {
                    std::cerr << "Invalid value: " << token.value << std::endl;
                    currentState = State::Error;
                }
            }
          
            else if (prevState == State::Value) {
               
                if (token.type == EType::Symbol && token.value == ".") {

                    SToken nextToken = m_tokenizer->getToken(input);
                    if (nextToken.type == EType::Word || nextToken.type == EType::Num) {
                        args[lastArg] += "." + nextToken.value;
                        std::cout << "DEBUG: Extended value for [" << lastArg << "]=" << args[lastArg] << std::endl;
                      
                        if (nextToken.type == EType::Word) {
                            currentState = State::Value;
                        }
                    } else {
                
                        currentState = State::Error;
                    }
                }
      
                else if (token.type == EType::Word) {
                    lastArg = token.value;
                    currentState = State::Arg;
                    std::cout << "DEBUG: New Argument=" << lastArg << std::endl;
                }
          
                else if (token.type == EType::Num) {
                    args[lastArg] += token.value; 
                }
                else {
                    currentState = State::Error;
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