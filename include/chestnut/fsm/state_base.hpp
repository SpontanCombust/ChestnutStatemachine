/**
 * @file state_base.hpp
 * @author Przemysław Cedro (SpontanCombust)
 * @brief Header file with fsm::StateBase
 * @version 3.0.0
 * @date 2022-01-30
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
 * @brief Base class of State with the most essential methods
 * 
 */
class StateBase
{
protected:
    /**
     * @brief Pointer to the statemachine that will house the state instance
     */
    StatemachineBase *parent;


public:
    /**
     * @brief Typedef of parent statemachine class to be used as class member type
     */
    typedef StatemachineBase StatemachineType;
    // Befriended fsm::Statemachine so that it can call setParent()
    friend StatemachineBase;
    /**
     * @brief Typedef of the base class (here it is this class itself)
     */
    typedef StateBase BaseStateType;
    

public:
    /**
     * @brief The constructor
     */
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
    /**
     * @brief Get the parent statemachine reference
     *  
     * @return parent statemachine reference
     * 
     * @throws BadParentAccessException if used before the parent pointer has been set (this will happen if used in a constructor)
     */
    virtual StatemachineBase& getParent();
    /**
     * @brief Get the parent statemachine reference
     * 
     * @return parent statemachine reference
     * 
     * @throws BadParentAccessException if used before the parent pointer has been set (this will happen if used in a constructor)
     */
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
    /**
     * @brief Set the parent pointer. This method is called internally from the statemachine
     * 
     * @param parent_ parent statemachine pointer
     * @return Returns whether this state type can be bound to a given statemachine type
     */
    virtual bool setParent( StatemachineBase *parent_ ) noexcept;
};

} // namespace chestnut::fsm


#include "state_base.inl"


#endif // __CHESTNUT_STATEMACHINE_STATE_BASE_H__