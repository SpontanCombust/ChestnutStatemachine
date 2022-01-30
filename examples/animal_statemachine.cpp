#include "animal_statemachine/animal.hpp"
#include "animal_statemachine/animal_state_idle.hpp"
#include "animal_statemachine/animal_state_moving.hpp"
#include "animal_statemachine/dog.hpp"
#include "animal_statemachine/dog_state_walking.hpp"

int main(int argc, char const *argv[])
{
    Animal animal;
    Dog dog;

    if( animal.gotoState<AnimalStateIdle>() )
    {
        puts("animal.gotoState<AnimalStateIdle>()");
        animal.performAction();
        puts("");
    }

    if( animal.gotoState<AnimalStateMoving>() )
    {
        puts("animal.gotoState<AnimalStateMoving>()");
        animal.performAction();
        puts("");
    }

    if( animal.gotoState<DogStateWalking>() )
    {
        puts("animal.gotoState<DogStateWalking>()");
        animal.performAction();
        puts("");
    }

    puts("");

    if( dog.gotoState<AnimalStateIdle>() )
    {
        puts("dog.gotoState<AnimalStateIdle>()");
        dog.performAction();
        puts("");
    }

    if( dog.gotoState<AnimalStateMoving>() )
    {
        puts("dog.gotoState<AnimalStateMoving>()");
        dog.performAction();
        puts("");
    }

    if( dog.gotoState<DogStateWalking>() )
    {
        puts("dog.gotoState<DogStateWalking>()");
        dog.performAction();
        puts("");
    }

    return 0;
}

/* CONSOLE OUTPUT
animal.gotoState<AnimalStateIdle>()
I am doing absolutely nothing!

animal.gotoState<AnimalStateMoving>()
I am moving now!


dog.gotoState<AnimalStateIdle>()
I am doing absolutely nothing!

dog.gotoState<AnimalStateMoving>()
I am moving now!

dog.gotoState<DogStateWalking>()
I am walking now!
(whispers) The secret number is 42.
*/