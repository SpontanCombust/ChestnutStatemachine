/**
 * @file state.hpp
 * @author SpontanCombust
 * @brief Header file with base template state class
 * @version 2.3.0
 * @date 2022-01-10
 * 
 * @copyright MIT License (c) 2021-2022
 * 
 */


#ifndef __CHESTNUT_STATEMACHINE_STATE_H__
#define __CHESTNUT_STATEMACHINE_STATE_H__

#include "state_transition.hpp"

#include <typeindex>

namespace chestnut::fsm
{

/**
 * @brief Base generic class used for creating state interface
 * 
 * @tparam ParentStatemachine type of the statemachine that will house the state
 * 
 * @details
 * ParentStatemachine type should be forward declared before the state interface.
 */
template< class ParentStatemachine >
class IState
{
protected:
    /**
     * @brief Pointer to the statemachine that houses the state
     */
    ParentStatemachine *parent;

public:
    typedef ParentStatemachine* ParentStatemachinePtrType;

public:
    IState( ParentStatemachine *parent_ );
    virtual ~IState() = default;
    
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
    virtual void onEnterState( StateTransition transition ) {}

    /**
     * @brief A method called whenever statemachine leaves this state
     * 
     * @details
     * gotoState, pushState and popState calls coming from this methods are ignored to prevent undefined behaviour. 
     * 
     * @param transition state transition data
     */
    virtual void onLeaveState( StateTransition transition ) {}
};

} // namespace chestnut::fsm


#include "state.inl"


#endif // __CHESTNUT_STATEMACHINE_STATE_H__