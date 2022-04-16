#pragma once

#include "../lumberjack.hpp"

#include <chestnut/fsm/state.hpp>


class LumberjackStateFinished : public chestnut::fsm::State<Lumberjack>
{
public:
    void onEnterState(chestnut::fsm::StateTransition transition) override
    {
        
    }
};