#pragma once

#include "animal.hpp"


struct DogStateExtension : AnimalStateExtension
{
    virtual const char *bark() = 0;
};

// WS: statemachine class Dog extends Animal
class Dog : public chestnut::fsm::Statemachine<DogStateExtension, Animal>
{
public:
    void bark()
    {
        puts( getCurrentState()->bark() );
    }
};