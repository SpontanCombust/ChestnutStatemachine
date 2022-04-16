/**
 * @example lumberjack_statemachine.cpp
 * @author Przemysław Cedro (SpontanCombust)
 * @brief A simulation of a game lumberjack game character
 * @details
 * Using the statemachine to simulate a behaviour of a lumberjack in a game revolving about gathering resources.
 * Each lumberjack is launched on a seperate thread. They go around the world harvesting wood from trees.
 * They have a limited wood capacity and have to drop it at the collection point from time to time.
 * Once every tree is harvested they finish their work.
 * @version 3.0.0
 * @date 2022-04-16
 * 
 * @copyright MIT License (c) 2021-2022
 * 
 * @include lumberjack_statemachine/forest.hpp
 * forest.hpp
 * @include lumberjack_statemachine/lumberjack.hpp
 * lumberjack.hpp
 * @include lumberjack_statemachine/lumberjack_states/searching.hpp
 * searching.hpp
 * @include lumberjack_statemachine/lumberjack_states/harvesting.hpp
 * harvesting.hpp
 * @include lumberjack_statemachine/lumberjack_states/collecting.hpp
 * collecting.hpp
 * @include lumberjack_statemachine/lumberjack_states/finished.hpp
 * finished.hpp
 */

#include "lumberjack_statemachine/forest.hpp"
#include "lumberjack_statemachine/lumberjack.hpp"
#include "lumberjack_statemachine/lumberjack_states/finished.hpp"
#include "lumberjack_statemachine/lumberjack_states/searching.hpp"
#include "lumberjack_statemachine/lumberjack_states/harvesting.hpp"
#include "lumberjack_statemachine/lumberjack_states/collecting.hpp"

int main(int argc, char const *argv[])
{
    Forest forest({
        // trees positions and wood counts
        {{1.f, 5.f}, 4},
        {{5.f, 8.f}, 3},
        {{-6.f, 1.f}, 2},
        {{0.f, -10.f}, 6},
    });

    Lumberjack lumberjack1(1, &forest, 1.f, 1.f, 4);
    lumberjack1.setPosition({0.f, 0.f});
    lumberjack1.setCollectionPoint({0.f, 0.f});
    Lumberjack lumberjack2(2, &forest, 1.5f, 0.6f, 6);
    lumberjack2.setPosition({5.f, 6.f});
    lumberjack2.setCollectionPoint({5.f, 0.f});

    std::thread t1([&lumberjack1]() {
        lumberjack1.startWork();
    });
    std::thread t2([&lumberjack2]() {
        lumberjack2.startWork();
    });

    t1.join();
    t2.join();

    return 0;
}

/* CONSOLE OUTPUT
Lumberjack 2 started working
Lumberjack 2 started searching for a tree to chop
Lumberjack 2 started walking to tree at (5.000000, 8.000000)
Lumberjack 1 started working
Lumberjack 1 started searching for a tree to chop
Lumberjack 1 started walking to tree at (1.000000, 5.000000)
Lumberjack 2 walked to tree at (5.000000, 8.000000)
Lumberjack 2 started harvesting a tree at (5.000000, 8.000000)
Lumberjack 1 walked to tree at (1.000000, 5.000000)
Lumberjack 1 started harvesting a tree at (1.000000, 5.000000)
Lumberjack 2 finished harvesting a tree at (5.000000, 8.000000)
Lumberjack 2 started searching for a tree to chop
Lumberjack 2 started walking to tree at (1.000000, 5.000000)
Lumberjack 1 has to drop the wood at the collection point (0.000000, 0.000000)
Lumberjack 1 started walking to the collection point
Lumberjack 2 decided to try change the target tree
Lumberjack 2 started walking to tree at (-6.000000, 1.000000)
Lumberjack 1 dropped the collected wood
Lumberjack 1 started searching for a tree to chop
Lumberjack 1 started walking to tree at (-6.000000, 1.000000)
Lumberjack 1 walked to tree at (-6.000000, 1.000000)
Lumberjack 1 started harvesting a tree at (-6.000000, 1.000000)
Lumberjack 2 walked to tree at (-6.000000, 1.000000)
Lumberjack 2 started harvesting a tree at (-6.000000, 1.000000)
Lumberjack 1 finished harvesting a tree at (-6.000000, 1.000000)
Lumberjack 1 started searching for a tree to chop
Lumberjack 1 started walking to tree at (0.000000, -10.000000)
Lumberjack 2 finished harvesting a tree at (-6.000000, 1.000000)
Lumberjack 2 started searching for a tree to chop
Lumberjack 2 started walking to tree at (0.000000, -10.000000)
Lumberjack 2 walked to tree at (0.000000, -10.000000)
Lumberjack 2 started harvesting a tree at (0.000000, -10.000000)
Lumberjack 1 walked to tree at (0.000000, -10.000000)
Lumberjack 1 started harvesting a tree at (0.000000, -10.000000)
Lumberjack 1 has to drop the wood at the collection point (0.000000, 0.000000)
Lumberjack 1 started walking to the collection point
Lumberjack 2 has to drop the wood at the collection point (5.000000, 0.000000)
Lumberjack 2 started walking to the collection point
Lumberjack 1 dropped the collected wood
Lumberjack 1 started searching for a tree to chop
Lumberjack 1 started walking to tree at (0.000000, -10.000000)
Lumberjack 2 dropped the collected wood
Lumberjack 2 started searching for a tree to chop
Lumberjack 2 started walking to tree at (0.000000, -10.000000)
Lumberjack 1 walked to tree at (0.000000, -10.000000)
Lumberjack 1 started harvesting a tree at (0.000000, -10.000000)
Lumberjack 1 finished harvesting a tree at (0.000000, -10.000000)
Lumberjack 1 started searching for a tree to chop
Lumberjack 1 finished their work
Lumberjack 2 decided to try change the target tree
Lumberjack 2 finished their work
*/