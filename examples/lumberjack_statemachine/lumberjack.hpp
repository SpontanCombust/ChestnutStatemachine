#pragma once

#include "vec2.hpp"
#include "forest.hpp"

#include <chestnut/fsm/statemachine.hpp>


// forward declarations
class LumberjackStateFinished;
class LumberjackStateSearching;
class LumberjackStateHarvesting;
class LumberjackStateCollecting;

class Lumberjack : public chestnut::fsm::Statemachine<>
{
    // making state classes friends so they have access to private members
    friend LumberjackStateFinished;
    friend LumberjackStateSearching;
    friend LumberjackStateHarvesting;
    friend LumberjackStateCollecting;

private:
    int id;
    Forest *forest;

    float walkingSpeed;
    float harvestingSpeed;
    int woodCapacity;

    vec2 collectionPoint;
    vec2 position;
    int woodCount;

public:
    // speed in units per second
    Lumberjack(int id, Forest *forest, float harvestingSpeed, float walkingSpeed, int woodCapacity)
    : id(id), forest(forest), harvestingSpeed(harvestingSpeed), walkingSpeed(walkingSpeed), woodCapacity(woodCapacity), woodCount(0)
    {
        initState<LumberjackStateFinished>();
    }

    Lumberjack& setPosition(vec2 pos)
    {
        this->position = pos;
        return *this;
    }

    Lumberjack& setCollectionPoint(vec2 pos)
    {
        this->collectionPoint = pos;
        return *this;
    }

    void startWork()
    {
        printf("Lumberjack %d started working\n", id);
        pushState<LumberjackStateSearching>();
    }
};