#pragma once

#include <chestnut/fsm/state.hpp>

#include "animal.hpp"

#include <cstdio>

// WS: state Moving in Animal
class AnimalStateMoving : public chestnut::fsm::State<Animal>
{
public:
    void performAction() override
    {
        puts("I am moving now!");
    }

protected:
    int getSecretNumber()
    {
        return 42;
    }
};