#pragma once

#include "../lumberjack.hpp"

#include <chestnut/fsm/state.hpp>

#include <limits>
#include <thread>

class LumberjackStateSearching : public chestnut::fsm::State<Lumberjack>
{
public:
    void onEnterState(chestnut::fsm::StateTransition transition) override
    {
        printf("Lumberjack %d started searching for a tree to chop\n", getParent().id);
        Tree *tree = pickClosestAvailableTree();    
        walkToTree(tree);
    }

    Tree *pickClosestAvailableTree() const
    {
        std::lock_guard<std::mutex> lock(getParent().forest->mutex);

        auto availableTrees = getParent().forest->getAvailableTrees();
        if (availableTrees.empty())
        {
            return nullptr;
        }

        Tree *tree = nullptr;
        float closest = std::numeric_limits<float>::max();
        for(auto available: availableTrees)
        {
            float dist = (available->position - getParent().position).length();
            if(dist < closest)
            {
                closest = dist;
                tree = available;
            }
        }

        return tree;
    }

    void walkToTree(Tree *tree)
    {
        if(!tree)
        {
            printf("Lumberjack %d finished their work\n", getParent().id);
            return (void)getParent().gotoState<LumberjackStateFinished>();
        }

        printf("Lumberjack %d started walking to tree at (%f, %f)\n", getParent().id, tree->position.x, tree->position.y);

        vec2 dirVec = (tree->position - getParent().position).normalized();
        float dist = (tree->position - getParent().position).length();
        bool shouldChangeTree = false;

        do
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(int(getParent().walkingSpeed * 1000)));

            {
                std::lock_guard<std::mutex> lock(getParent().forest->mutex);

                if(tree->woodCount == 0)
                {
                    shouldChangeTree = true;
                    break;
                }
            }

            getParent().position += dirVec * std::min(getParent().walkingSpeed, dist);
            dist = (tree->position - getParent().position).length();
            // printf("Lumberjack %d is now at position (%f, %f)\n", getParent().id, getParent().position.x, getParent().position.y);

        } while (dist >= 0.1f);

        // tree was harvested while the lumberjack was walking
        if(shouldChangeTree)
        {
            printf("Lumberjack %d decided to try change the target tree\n", getParent().id);
            walkToTree(pickClosestAvailableTree());
        }
        else
        {
            printf("Lumberjack %d walked to tree at (%f, %f)\n", getParent().id, tree->position.x, tree->position.y);
            getParent().position = tree->position;
            return (void)getParent().pushState<LumberjackStateHarvesting>(tree);
        }
    }
};