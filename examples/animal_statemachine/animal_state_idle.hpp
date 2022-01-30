#pragma once

#include <chestnut/fsm/state.hpp>

#include "animal.hpp"

#include <cstdio>

// WS: state Idle in Animal
class AnimalStateIdle : public chestnut::fsm::State<Animal>
{
public:
    void performAction() override
    {
        puts("I am doing absolutely nothing!");
    }
};