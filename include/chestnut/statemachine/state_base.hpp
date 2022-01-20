/**
 * @file state_base.hpp
 * @author Przemysław Cedro (SpontanCombust)
 * @brief Header file with the base class of IState
 * @version 3.0.0
 * @date 2022-01-20
 * 
 * @copyright MIT License (c) 2021-2022
 * 
 */

#ifndef __CHESTNUT_STATEMACHINE_STATE_BASE_H__
#define __CHESTNUT_STATEMACHINE_STATE_BASE_H__

#include "state_transition.hpp"

namespace chestnut::fsm
{

/**
 * @brief Base class of IState, housing the methods that were not template type-specific to IState
 * 
 */
class IState_Base
{
public:
    /**
     * @brief Virtual IState destructor, it's marked as default
     */
    virtual ~IState_Base() = default;

    /**
     * @brief A method used to evaluate if a state is able to transition from other specified state
     * 
     * @param transition state transition data
     * @return if can transition to some state
     * 
     * @details
     * By default this always returns true
     */
    virtual bool canEnterState( StateTransition transition ) const noexcept;

    /**
     * @brief A method used to evaluate if a state is able to transition to other specified state
     * 
     * @param transition state transition data
     * @return if can transition from some state
     * 
     * @details
     * By default this always returns true
     */
    virtual bool canLeaveState( StateTransition transition ) const noexcept;
    
    /**
     * @brief A method called whenever statemachine enters this state
     * 
     * @param transition state transition data
     */
    virtual void onEnterState( StateTransition transition );

    /**
     * @brief A method called whenever statemachine leaves this state
     * 
     * @details
     * gotoState, pushState and popState calls coming from this methods are ignored to prevent undefined behaviour. 
     * 
     * @param transition state transition data
     */
    virtual void onLeaveState( StateTransition transition );
};

} // namespace chestnut::fsm


#include "state_base.inl"


#endif // __CHESTNUT_STATEMACHINE_STATE_BASE_H__