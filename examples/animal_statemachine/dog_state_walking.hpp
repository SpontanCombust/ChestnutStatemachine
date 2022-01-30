#pragma once

#include "dog.hpp"

// WS: state Walking in Dog extends Moving
class DogStateWalking : public chestnut::fsm::State<Dog, AnimalStateMoving>
{
public:
    void performAction() override
    {
        puts("I am walking now!");
        printf("(whispers) The secret number is %d.\n", getSecretNumber() );
    }

    const char *bark() override
    {
        return "Woof!";
    }
};