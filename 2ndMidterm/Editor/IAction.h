// IAction.h
#pragma once

class Presentation;

class IAction {
public:
    virtual ~IAction() = default;

    virtual void doAction(Presentation& doc) = 0;
    virtual void undoAction(Presentation& doc) = 0;
};