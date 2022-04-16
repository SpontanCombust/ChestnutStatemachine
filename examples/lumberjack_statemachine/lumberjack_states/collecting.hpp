#pragma once

#include "../lumberjack.hpp"

#include <chestnut/fsm/state.hpp>


class LumberjackStateCollecting : public chestnut::fsm::State<Lumberjack>
{
public:
    void onEnterState(chestnut::fsm::StateTransition transition) override
    {
        printf("Lumberjack %d started walking to the collection point\n", getParent().id);
        walkToCollectionPoint();
        getParent().woodCount = 0;
        printf("Lumberjack %d dropped the collected wood\n", getParent().id);
        return (void)getParent().gotoState<LumberjackStateSearching>();
    }

    void walkToCollectionPoint()
    {
        vec2 dirVec = (getParent().collectionPoint - getParent().position).normalized();
        float dist = (getParent().collectionPoint - getParent().position).length();

        do
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(int(getParent().walkingSpeed * 1000)));
            getParent().position += dirVec * std::min(getParent().walkingSpeed, dist);
            dist = (getParent().collectionPoint - getParent().position).length();

        } while (dist >= 0.1f);

        getParent().position = getParent().collectionPoint;
    }
};