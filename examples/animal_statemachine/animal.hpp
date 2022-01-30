#pragma once

#include <chestnut/fsm/statemachine.hpp>


struct AnimalStateExtension
{
    virtual void performAction() = 0;
};

// WS: statemachine class Animal
class Animal : public chestnut::fsm::Statemachine<AnimalStateExtension>
{
public:
    void performAction() 
    {
        getCurrentState()->performAction();
    }
};