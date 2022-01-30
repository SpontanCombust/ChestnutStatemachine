/**
 * @file state_base.hpp
 * @author Przemysław Cedro (SpontanCombust)
 * @brief Header file with fsm::StateBase - the most base class to create a state class
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

// forward declaration because of mutual dependence
class StatemachineBase;


/**
 * @brief Base class of State, housing the methods that don't depend on state's parent statemachine type
 * 
 */
class StateBase
{
protected:
    StatemachineBase *parent;


public:
    /**
     * @brief Typedef of parent statemachine class to be used as class member type
     */
    typedef StatemachineBase StatemachineType;
    // Befriended fsm::Statemachine so that it can call setParent()
    friend StatemachineBase;

    typedef StateBase BaseStateType;
    

public:
    StateBase();
    
    /**
     * @brief Virtual State destructor, it's marked as default
     */
    virtual ~StateBase() = default;

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
    


protected:
    // Don't call in constructor! BadParentAccessException
    virtual StatemachineBase& getParent();
    // Don't call in constructor! BadParentAccessException
    virtual const StatemachineBase& getParent() const;

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


private:
    // Returns whether this state type can be bound to a given statemachine type
    virtual bool setParent( StatemachineBase *parent_ ) noexcept;
};

} // namespace chestnut::fsm


#include "state_base.inl"


#endif // __CHESTNUT_STATEMACHINE_STATE_BASE_H__