#pragma once

#include "vec2.hpp"

#include <mutex>
#include <list>

struct Tree
{
    vec2 position;
    unsigned int woodCount;
};

class Forest
{
public:
    std::list<Tree> trees;
    std::mutex mutex;

public:
    Forest(std::list<Tree> trees)
    {
        this->trees = trees;
    }

    std::list<Tree *> getAvailableTrees()
    {
        std::list<Tree *> available;
        for(Tree& tree : this->trees)
        {
            if(tree.woodCount > 0)
            {
                available.push_back(&tree);
            }
        }

        return available;
    }
};