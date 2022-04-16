#pragma once

#include "../lumberjack.hpp"

#include <chestnut/fsm/state.hpp>


class LumberjackStateHarvesting : public chestnut::fsm::State<Lumberjack>
{
private:
    Tree *harvestedTree;

public:
    LumberjackStateHarvesting(Tree *tree)
        : harvestedTree(tree)
    {

    }

    void onEnterState(chestnut::fsm::StateTransition transition) override
    {
        printf("Lumberjack %d started harvesting a tree at (%f, %f)\n", getParent().id, harvestedTree->position.x, harvestedTree->position.y);
        harvest();
    }

    bool isHarvested() const
    {
        std::lock_guard<std::mutex> lock(getParent().forest->mutex);
        return harvestedTree->woodCount == 0;
    }

    void harvest()
    {
        while(!isHarvested())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(int(getParent().harvestingSpeed * 1000)));

            {
                std::lock_guard<std::mutex> lock(getParent().forest->mutex);

                if(harvestedTree->woodCount > 0 && getParent().woodCount < getParent().woodCapacity)
                {
                    harvestedTree->woodCount--;
                    getParent().woodCount++;
                }
                if(getParent().woodCount >= getParent().woodCapacity)
                {
                    break;
                }
            }
        }

        if(getParent().woodCount >= getParent().woodCapacity)
        {
            printf("Lumberjack %d has to drop the wood at the collection point (%f, %f)\n", getParent().id, getParent().collectionPoint.x, getParent().collectionPoint.y);
            return (void)getParent().gotoState<LumberjackStateCollecting>();
        }
        else
        {
            printf("Lumberjack %d finished harvesting a tree at (%f, %f)\n", getParent().id, harvestedTree->position.x, harvestedTree->position.y);
            return (void)getParent().popState();
        }
    }
};